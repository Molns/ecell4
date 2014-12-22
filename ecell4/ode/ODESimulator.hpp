#ifndef __ECELL4_ODE_ODE_SIMULATOR_HPP
#define __ECELL4_ODE_ODE_SIMULATOR_HPP

#include <cstring>
#include <vector>
#include <boost/shared_ptr.hpp>

#include <ecell4/core/exceptions.hpp>
#include <ecell4/core/types.hpp>
#include <ecell4/core/get_mapper_mf.hpp>
#include <ecell4/core/NetworkModel.hpp>
#include <ecell4/core/SimulatorBase.hpp>
#include <ecell4/core/ModelWrapper.hpp>

#include "ODEWorld.hpp"

#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

namespace ecell4
{

namespace ode
{

class ODESimulator
    : public SimulatorBase<Model, ODEWorld>
{
public:

    typedef SimulatorBase<Model, ODEWorld> base_type;

public:

    typedef boost::numeric::ublas::vector<double> state_type;
    typedef boost::numeric::ublas::matrix<double> matrix_type;
    typedef std::vector<state_type::size_type> index_container_type;

    typedef struct
    {
        index_container_type reactants;
        index_container_type products;
        Real k;
        boost::weak_ptr<Ratelaw> ratelaw;
    } reaction_type;

    typedef std::vector<reaction_type> reaction_container_type;

    class deriv_func
    {
    public:

        deriv_func(const reaction_container_type& reactions, const Real& volume)
            : reactions_(reactions), volume_(volume), vinv_(1.0 / volume)
        {
            ;
        }

        void operator()(const state_type& x, state_type& dxdt, const double& t)
        {
            for (state_type::iterator i(dxdt.begin()); i != dxdt.end(); ++i)
            {
                *i = 0.0;
            }
            // XXX
            for (reaction_container_type::const_iterator
                i(reactions_.begin()); i != reactions_.end(); i++)
            {
                // Prepare  state_array of reactants and products that contain amounts of each reactants.
                Ratelaw::state_container_type reactants_states(i->reactants.size());
                Ratelaw::state_container_type products_states(i->products.size());
                Ratelaw::state_container_type::size_type cnt(0);

                for (index_container_type::const_iterator
                    j((*i).reactants.begin()); j != (*i).reactants.end(); ++j, cnt++)
                {
                    reactants_states[cnt] = x[*j];
                }
                cnt = 0;
                for (index_container_type::const_iterator
                    j((*i).products.begin()); j != (*i).products.end(); ++j, cnt++)
                {
                    products_states[cnt] = x[*j];
                }

                double flux;
                // Get pointer of Ratelaw object and call it.
                if (i->ratelaw.expired() || i->ratelaw.lock()->is_available() == false)
                {
                    boost::scoped_ptr<Ratelaw> temporary_ratelaw_obj(new RatelawMassAction(i->k));
                    flux = temporary_ratelaw_obj->deriv_func(reactants_states, products_states, volume_);
                }
                else
                {
                    boost::shared_ptr<Ratelaw> ratelaw = (*i).ratelaw.lock();
                    flux = (*ratelaw).deriv_func(reactants_states, products_states, volume_);
                }

                // Merge each reaction's flux into whole dxdt
                for (index_container_type::const_iterator
                    j((*i).reactants.begin()); j != (*i).reactants.end(); ++j)
                {
                    dxdt[*j] -= flux;
                }
                for (index_container_type::const_iterator
                    j((*i).products.begin()); j != (*i).products.end(); ++j)
                {
                    dxdt[*j] += flux;
                }
            }
        }

    protected:

        const reaction_container_type reactions_;
        const Real volume_;
        const Real vinv_;
    };

    class jacobi_func
    {
    public:

        jacobi_func(const reaction_container_type& reactions, const Real& volume)
            : reactions_(reactions), volume_(volume), vinv_(1.0 / volume)
        {
            ;
        }

        void operator()(
            const state_type& x, matrix_type& jacobi, const double& t, state_type& dfdt) const
        {
            // fill 0 into jacobi and dfdt
            for (state_type::iterator i(dfdt.begin()); i != dfdt.end(); ++i)
            {
                *i = 0.0;
            }
            for (matrix_type::array_type::iterator i(jacobi.data().begin());
                i != jacobi.data().end(); ++i)
            {
                *i = 0.0;
            }

            // Calculate jacobian for each reaction and merge it.
            for (reaction_container_type::const_iterator
                i(reactions_.begin()); i != reactions_.end(); i++)
            {
                // Calculate a reaction's jacobian.
                // prepare state_array that contain amounts of reactants
                index_container_type::size_type reactants_size(i->reactants.size());
                index_container_type::size_type products_size(i->products.size());
                Ratelaw::state_container_type reactants_states(reactants_size);
                Ratelaw::state_container_type products_states(products_size);
                Ratelaw::state_container_type::size_type cnt(0);
                for (index_container_type::const_iterator
                    j((*i).reactants.begin()); j != (*i).reactants.end(); ++j, cnt++)
                {
                    reactants_states[cnt] = x[*j];
                }
                cnt = 0;
                for (index_container_type::const_iterator
                    j((*i).products.begin()); j != (*i).products.end(); ++j, cnt++)
                {
                    products_states[cnt] = x[*j];
                }
                // prepare matrix object that will be filled with numerical differentiate.
                matrix_type::size_type row_length = reactants_size + products_size;
                matrix_type::size_type col_length = row_length;
                matrix_type mat(row_length, col_length);

                // get the pointer of Ratelaw object and call it.
                if (i->ratelaw.expired() || i->ratelaw.lock()->is_available() == false)
                {
                    boost::scoped_ptr<Ratelaw> temporary_ratelaw_obj(new RatelawMassAction(i->k));
                    temporary_ratelaw_obj->jacobi_func(mat, reactants_states, products_states, volume_);
                }
                else
                {
                    boost::shared_ptr<Ratelaw> ratelaw = (*i).ratelaw.lock();
                    (*ratelaw).jacobi_func(mat, reactants_states, products_states, volume_);
                }

                //merge jacobian
                for(matrix_type::size_type row(0); row < row_length; row++)
                {
                    matrix_type::size_type j_row(row < reactants_size ? (*i).reactants[row] : (*i).products[row - reactants_size]);
                    for(matrix_type::size_type col(0); col < col_length; col++)
                    {
                        matrix_type::size_type j_col(col < reactants_size ? (*i).reactants[col] : (*i).products[col - reactants_size]);
                        jacobi(j_row, j_col) += mat(row, col);
                    }
                }
            }
        }

    protected:

        const reaction_container_type reactions_;
        const Real volume_;
        const Real vinv_;
    };

    struct StateAndTimeBackInserter
    {
        typedef std::vector<state_type> state_container_type;
        typedef std::vector<double> time_container_type;

        state_container_type& m_states;
        time_container_type& m_times;

        StateAndTimeBackInserter(
            state_container_type& states, time_container_type& times)
            : m_states(states), m_times(times)
        {
            ;
        }

        void operator()(const state_type&x, double t)
        {
            m_states.push_back(x);
            m_times.push_back(t);
        }
    };

public:

    ODESimulator(
        const boost::shared_ptr<Model>& model,
        const boost::shared_ptr<ODEWorld>& world)
        : base_type(model, world), dt_(inf)
    {
        initialize();
    }

    ODESimulator(const boost::shared_ptr<ODEWorld>& world)
        : base_type(world), dt_(inf)
    {
        initialize();
    }

    void initialize()
    {
        boost::shared_ptr<Model> expanded(model_->expand(world_->list_species()));
        if (!expanded)
        {
            throw IllegalState("The model failed to be expanded.");
        }
        expanded_.swap(expanded); // use expanded_, but not model_

        const std::vector<Species> species(expanded_->list_species());
        for (std::vector<Species>::const_iterator
            i(species.begin()); i != species.end(); ++i)
        {
            if (!(*world_).has_species(*i))
            {
                (*world_).reserve_species(*i);
            }
        }
    }

    // SimulatorTraits

    Real t(void) const
    {
        return (*world_).t();
    }

    Real dt() const
    {
        return dt_;
    }

    void step(void)
    {
        step(next_time());
    }

    bool step(const Real& upto);

    // Optional members

    void set_t(const Real& t)
    {
        (*world_).set_t(t);
    }

    void set_dt(const Real& dt)
    {
        if (dt <= 0)
        {
            throw std::invalid_argument("The step size must be positive.");
        }
        dt_ = dt;
    }

protected:

    std::pair<deriv_func, jacobi_func> generate_system() const;

protected:

    Real dt_;
    boost::shared_ptr<Model> expanded_;
};

} // ode

} // ecell4

#endif /* __ECELL4_ODE_ODE_SIMULATOR_HPP */
