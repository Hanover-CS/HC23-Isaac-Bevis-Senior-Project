// define namespace

#ifndef BEVIS_ROLLINGCODES
#define BEVIS_ROLLINGCODES

namespace bevis_FinalProject 
{
class RollingCode 
{
   int seed;

   public: RollingCode(int s=-1){
      seed = s;
   }

   public: int getSeed(){
      return seed;
   }
   

}; // class rollingCode

} // namespace bevis_FinalProject 

#endif // ifndef BEVIS_ROLLINGCODES