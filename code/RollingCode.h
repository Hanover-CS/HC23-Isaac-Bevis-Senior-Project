   /*
   RollingCode.h
   Defines the RollingCode Class that can be used to generate random codes for key-lock verification
   Using Linear congruential generator algorithm
   Where X[n+1] = (a * X[n] + c) % m
         m, 0 < m -- the "modulus"
         a, 0 < a < m -- the "multiplier"
         c, 0 <= c < m -- the "increment"
         X[0], 0 <= X[0] < m -- the start value (starting seed)
   This uses the popular c != 0 method where 
      m and c are relatively prime, 
      and a-1 is divisible by all prime factors of m,
      and a-1 is divisible by 4 if m is.
   */

#ifndef BEVIS_ROLLINGCODES
#define BEVIS_ROLLINGCODES

namespace bevis_FinalProject 
{
class RollingCode 
{

   const int SEARCH_LIMIT = 100;

   unsigned long seed;

   // default values calculated using TestPrograms/calculateMCA.py
   unsigned long m;
   unsigned long a;
   unsigned long c;

   // Constructor
   // Creates rolling code with seed s using the Linear congruential generator
   //       that uses M A and C
   public: RollingCode(unsigned long s, 
                     unsigned long M=4152143978, 
                     unsigned long A=9554, 
                     unsigned long C=5903) {
      seed = s;
      m = M;
      a = A;
      c = C;
   }

   // function next
   // returns: the next code in the pseudo-random sequence
   // Post Condition: seed has been updated to the next code
   public: unsigned long next() {
      seed = (a * seed + c) % m;
      return seed;
   }

   // function matches
   // input: a unsigned long representing the code to be compared to
   // returns: true or false depending on if the code matches one of the next
   //       100 codes in the pseudo-random sequence
   // Post condition: seed has been updated to the matched code
   //       so that de-synced codes can be re-synced
   public: bool matches(unsigned long code) {
      // create temp code at same location in pseudo-random sequence
      //       so that seed is only modified if the code matches
      RollingCode tempCode = RollingCode(seed, m, a, c); 
      for (int i=0; i<=SEARCH_LIMIT; i++){
         if (tempCode.getSeed() == code) {
            seed = tempCode.getSeed();
            return true;
         }
         tempCode.next();
      }
      return false;
   }

   // function getSeed
   // returns: the current seed of this rolling code
   public: unsigned long getSeed() {
      return seed;
   }
   

}; // class rollingCode

} // namespace bevis_FinalProject 

#endif // ifndef BEVIS_ROLLINGCODES