// define namespace

#ifndef BEVIS_ROLLINGCODES
#define BEVIS_ROLLINGCODES

namespace bevis_FinalProject 
{
class RollingCode 
{
   /*
   Using Linear congruential generator algorithm
   Where X[n+1] = (a * X[n] + c) % m
         m, 0 < m -- the "modulus"
         a, 0 < a < m -- the "multiplier"
         c, 0 <= c < m -- the "increment"
         X[0], 0 <= X[0] < m -- the start value (the seed)
   This uses the popular c != 0 method where 
      m and c are relatively prime, 
      and a-1 is divisible by all prime factors of m,
      and a-1 is divisible by 4 if m is.
   */
   unsigned long seed;

   // default values calculated using TestPrograms/calculateMCA.py
   unsigned long m;
   unsigned long a;
   unsigned long c;

   public: RollingCode(unsigned long s, 
                     unsigned long M=4152143978, 
                     unsigned long A=9554, 
                     unsigned long C=5903) {
      seed = s;
      m = M;
      a = A;
      c = C;
   }

   public: unsigned long next(){
      seed = (a * seed + c) % m;
      return seed;
   }

   public: unsigned long getSeed(){
      return seed;
   }
   

}; // class rollingCode

} // namespace bevis_FinalProject 

#endif // ifndef BEVIS_ROLLINGCODES