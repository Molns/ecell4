#!/usr/bin/env python

from egfrd import *
from bd import *

from logger import *
import sys
import time
import model
import gfrdbase
import _gfrd
import myrandom

def run_single(T, V, N):

    print 'T =', T, '; V= ', V, '; N=', N
    
    # disable gc
    import gc
    gc.disable()

    L = math.pow(V * 1e-3, 1.0 / 3.0)

    matrix_size = max(3, int((3 * N) ** (1.0/3.0)))

    print 'matrix_size=', matrix_size
    
    D = 1e-12

    m = model.ParticleModel(L)
    A = model.Species('A', D, 2.5e-9)
    m.add_species_type(A)
    m.set_all_repulsive()

    w = gfrdbase.create_world(m, matrix_size)
    nrw = _gfrd.NetworkRulesWrapper(m.network_rules)
    #s = EGFRDSimulator(w, myrandom.rng, nrw)
    myrandom.seed(3964642685656813207)
    s = _gfrd._EGFRDSimulator(w, nrw, myrandom.rng)
    #s.paranoiac = True

    gfrdbase.throw_in_particles(w, A, N)
    print 'stir'

    stir_time = T * .1
    while 1:
        s.step()
        next_time = s.t + s.dt
        if next_time > stir_time:
            s.step(stir_time)
            break

    stir_steps = s.num_steps

    #print 'reset'
    #s.reset()

    print 'run'
    run_time = T + stir_time

    start = time.time()
    while s.t < run_time:
        s.step()
    end = time.time()
    timing = end - start

    steps = s.num_steps - stir_steps
    stepspersec = float(steps) / timing
    print 'steps (total)= ', steps
    print 'steps/sec= ', stepspersec, ', steps/N= ', float(steps) / N
    print 'TIMING:\n', timing, '\n'

    gc.collect()
    gc.enable()

    return end - start, steps, stepspersec


def run_single_bd(T, V, N, dt_factor):

    print 'T =', T, '; V= ', V, '; N=', N
    
    # disable gc
    import gc
    gc.disable()

    L = math.pow(V * 1e-3, 1.0 / 3.0)

    matrix_size = max(3, int((3 * N) ** (1.0/3.0)))

    print 'matrix_size=', matrix_size
    
    D = 1e-12

    m = model.ParticleModel(L)
    A = model.Species('A', D, 2.5e-9)
    m.add_species_type(A)
    m.set_all_repulsive()

    w = gfrdbase.create_world(m, matrix_size)
    nrw = _gfrd.NetworkRulesWrapper(m.network_rules)
    s = _gfrd._BDSimulator(w, nrw, myrandom.rng, dt_factor)
    
    gfrdbase.throw_in_particles(w, A, N)
    print 'stir'

    t = 0
    stir_time = T * .1
    while 1:
        s.step()
        next_time = s.t + s.dt
        if next_time > stir_time:
            s.step(stir_time)
            break

    stir_steps = s.num_steps

    #print 'reset'
    #s.reset()

    print 'run'
    run_time = T + stir_time

    start = time.time()
    while s.t < run_time:
        s.step()
    end = time.time()

    timing = end - start

    steps = s.num_steps - stir_steps
    stepspersec = float(steps) / timing
    print 'steps (total)= ', steps
    print 'steps/sec= ', stepspersec, ', steps/N= ', float(steps) / N
    print 'TIMING:\n', timing, '\n'

    gc.collect()
    gc.enable()

    return end - start, steps, stepspersec



if __name__ == '__main__':
    
    T = float(sys.argv[1])
    V = float(sys.argv[2])
    N = int(sys.argv[3])

    run_single(T, V, N)

