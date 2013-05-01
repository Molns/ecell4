

cdef class RandomNumberGenerator:
    #cdef Cpp_GSLRandomNumberGenerator *thisptr
    def __cinit__(self):
        self.thisptr = new Cpp_GSLRandomNumberGenerator()
            
    def __dealloc__(self):
        del self.thisptr

    def uniform(self, Real min, Real max):
        return self.thisptr.uniform(min, max)
    def uniform_inf(self, Integer min, Integer max):
        return self.thisptr.uniform_int(min, max)
    def gaussian(self, Real mean, Real sigma):
        return self.thisptr.gaussian(mean, sigma)
    def seed(self, Integer val):
        self.thisptr.seed(val)