#!/usr/bin/env python
import _gfrd
import math
import random

import numpy

#print gfrd.distance_sq(numpy.array((1.,2.,3.)), numpy.array((4.,5.,6., )))

def test_draw_time_single():

    D = 1e-12

    a = 1e-7

    gf = _gfrd.GreensFunction3DAbsSym(D)
    #gf.seta(a)
    
    for i in range(150000):
        rnd = random.random()
        t = gf.drawTime(rnd, a)

    t = gf.drawTime(.5, a)
    print t


def test_drawR_single():

    D = 1e-12
    t = 1.38e-3

    a = 1e-7

    gf = _gfrd.GreensFunction3DAbsSym(D)
    
    for i in range(50000):
        rnd = random.random()
        r = gf.drawR(rnd, t, a)

    r = gf.drawR(.5, t, a)
    print r




def test_alpha0():

    D = 1e-12
    Sigma = 1e-8
    kf = 1e-18

    a = 2e-7

    gf = _gfrd.GreensFunction3DRadAbs(D, kf, Sigma)
    gf.seta(a)
    maxerror = 0

    for i in range(100):
        alpha = gf.alpha0_i(i)
        error = abs(gf.f_alpha0(alpha))
        maxerror = max(error, maxerror)

    if abs(maxerror) > 1e-8:
        print 'failed: alpha_survival_n: maxerror = ', maxerror


def test_alpha():

    D = 1e-12
    Sigma = 1e-8
    kf = 1e-18

    a = 2e-7

    gf = _gfrd.GreensFunction3DRadAbs(D, kf, Sigma)
    gf.seta(a)
    maxerror = 0
    
    n = 30
    for i in range(1,200):
        alpha = gf.alpha_i(i, n)
        error = abs(gf.f_alpha(alpha, n))
        #error = abs(gf.f_alpha0(alpha))
        print error
        maxerror = max(error, maxerror)

    if abs(maxerror) > 1e-8:
        print 'alpha_i: maxerror = ', maxerror


def test_p_survival():

    t = 1e-7
    D = 1e-12
    Sigma = 1e-8
    kf = 1e-18

    r0 = 5e-8
    a = 6e-8

    gf = _gfrd.GreensFunction3DRadAbs(D, kf, Sigma)
    gf.seta(a)
    
    for i in range(1000):
        gf.p_survival(t, r0)

    print gf.p_survival(t, r0)



def test_draw_time():

    D = 1e-12
    Sigma = 1e-8
    kf = 1e-8

    r0 = 5e-8
    a = 1e-5

    gf = _gfrd.GreensFunction3DRadAbs(D, kf, Sigma)
    gf.seta(a)
    
    #for i in range(20000):
    #    rnd = random.random()
    #    t = gf.drawTime(rnd, r0)
    #    gf.drawEventType(rnd, r0, t)

    t = gf.drawTime(.5, r0)
    type = gf.drawEventType(.5, r0, t)
    print t, type


def test_drawR():

    t = 1e-8
    D = 1e-12
    Sigma = 1e-8
    kf = 1e-8

    r0 = 5e-8
    a = 1e-7

    gf = _gfrd.GreensFunction3DRadAbs(D, kf, Sigma)
    gf.seta(a)
    
    for i in range(1000):
        rnd = random.random()
        t = gf.drawR(rnd, r0, t)

    t = gf.drawR(rnd, r0, t)
    print t


def test_f_alpha():

    t = 1e-8
    D = 1e-12
    Sigma = 1e-8
    kf = 1e-18

    r0 = 5e-8
    a = 1e-7

    gf = _gfrd.GreensFunction3DRadAbs(D, kf, Sigma)
    gf.seta(a)
    
    n = 0
    for i in range(1,1000):
        f = gf.f_alpha(1e6 * i, n)
        print f


def test_draw_theta():

    t = 1e-7
    D = 1e-12
    Sigma = 5e-9
    kf = 1e-13

    r = 1.05e-8
    r0 = 1.05e-8
    a = 1.1e-8
    
    gf = _gfrd.GreensFunction3DRadAbs(D, kf, Sigma)
    gf.seta(a)
    
    for i in range(20000):
        rnd = random.random()
        theta = gf.drawTheta(rnd, r, r0, t)

    theta = gf.drawTheta(0.5, r, r0, t)
    print theta


def test_draw_theta_at_a():
    
    t = 1e-3
    D = 1e-12
    Sigma = 1e-8
    kf = 1e-8

    r0 = 0.999e-7
    a = 1e-7
    
    gf = _gfrd.GreensFunction3DRadAbs(D, kf, Sigma)
    gf.seta(a)
    
#    for i in range(200):
#        rnd = random.random()
#        theta = gf.drawTheta(rnd, r, r0, t)

    theta = gf.drawTheta(0.5, a, r0, t)
    print theta


    
#for i in range(1000):
#    print gf.drawR(0.9, r0, t)


#test_alpha0()


#test_p_survival()
#test_draw_time()
#test_drawR()

#test_draw_time_single()
#test_drawR_single()

#test_f_alpha()



#test_draw_theta()
#test_draw_theta_at_a()

#test_alpha()

