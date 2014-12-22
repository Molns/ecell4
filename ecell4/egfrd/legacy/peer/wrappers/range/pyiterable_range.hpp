#ifndef PEER_WRAPPERS_PYITERABLE_RANGE_HPP
#define PEER_WRAPPERS_PYITERABLE_RANGE_HPP

#include <boost/python.hpp>
#include <boost/optional.hpp>
#include <boost/range/size.hpp>
#include <boost/range/difference_type.hpp>
#include <boost/mpl/bool.hpp>
#include "generator.hpp"
#include "utils/range.hpp"

#include "peer/wrappers/generator/pyiterator_generator.hpp"

namespace peer { namespace wrappers {

template<typename Tvalue_>
struct pyiterable_range
{
    typedef pyiterator_generator<Tvalue_> generator_type;

    struct generator_holder
    {
        bool operator==(generator_holder const& rhs) const
        {
            return impl_ == rhs.impl_;
        }

        operator bool() const
        {
            return impl_.is_initialized();
        }

        generator_type* operator->() const
        {
            return boost::get_pointer(impl_);
        }

        generator_type& operator*() const
        {
            return boost::get(impl_);
        }

        generator_holder(generator_type const& impl): impl_(impl) {}

        generator_holder(): impl_() {}

        mutable boost::optional<generator_type> impl_;
    };

    typedef generator_iterator<generator_type, generator_holder> iterator;
    typedef typename boost::remove_reference<
            typename generator_type::result_type> value_type;
    typedef typename generator_type::result_type reference;
    typedef iterator const_iterator;

    pyiterable_range(boost::python::object obj): obj_(obj) {}

    std::size_t size() const
    {
        return PyObject_Size(obj_.ptr());
    }

    iterator begin() const
    {
        return iterator(generator_holder(generator_type(
            boost::python::handle<>(PyObject_GetIter(obj_.ptr())))));
    }

    iterator end() const
    {
        return iterator();
    }

protected:
    boost::python::object obj_;
};

} } // namespace peer::wrappers

template<typename Tvalue_>
struct range_size<peer::wrappers::pyiterable_range<Tvalue_> >
{
    typedef std::size_t type;
};

template<typename Tvalue_>
struct range_size_retriever<peer::wrappers::pyiterable_range<Tvalue_> >
{
    typedef peer::wrappers::pyiterable_range<Tvalue_> argument_type;
    typedef typename range_size<argument_type>::type result_type;

    result_type operator()(argument_type const& range) const
    {
        return range.size();
    }
};

template<typename Tvalue_>
struct is_sized<peer::wrappers::pyiterable_range<Tvalue_> >: public boost::mpl::true_
{
};

#endif /* PEER_WRAPPERS_PYITERABLE_RANGE_HPP */
