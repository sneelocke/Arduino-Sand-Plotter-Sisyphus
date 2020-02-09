/* polar sand plotter

 10/24/17 By A. Kinsman while at Sanibel playing with the sand
 plotting machine.
 
*/ 

#include <Servo.h> 

#define MS_PER_ROTATION 202500  // Time for table to rotate
                                // The 3 RPM motor is not really that speed.  
                                // The table rotates 10 times in 33'45".
                                // Once in 3'22.5", not the predicted 3'20"
                                // The motor is slow.
                                // The motor is really 2.9629 RPM!
                                
#define MAX_US 370              // cardboard plate 370 - longest wave sent to servo
                                // glass plate 300
                                // yellow plate 250
                                // clear plate 300
                                
#define ERASE_RAD MAX_US        // erase radius - used for erase only
#define MAX_RAD (ERASE_RAD-20)  // largest radius for drawings
#define MAX_YELLOW_PLATE_RAD (MAX_RAD*10/15)  // for use with 10" diameter yellow plate
#define INCH_RAD (ERASE_RAD/7.5)// counts per inch of radius
#define DISPLAY_TIME 15         // minutes for display before erase
#define SERVO_DELAY  50         // Most set_servo calls are same for computing continual scale reductions
#define CIRC_RAD (MS_PER_ROTATION/15.0*PI)  // ms per inch of rotation at max radius
#define MAX_TURNS     40        // number of times to turn the table for a drawing
#define REDUCE_PER_TURN .955    // nice reduce per turn
#define REDUCE_PER_PLOT .999995 // nice reduce per 50ms plot
#define ERASE    1              // on/off for debugging wears out servo
#define MAKER    0              // maker faire code

Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
float scale = 1.0;  // used for reduction drawings
float r,a,b = 0.0;  // used for R in polar plots,a for angle
int   i,j,k = 0;    // temps

unsigned long start_time = 0;  // time out program starts, zero time for table angle

// cosine in degrees
float cosD(float angle)
{
    return( cos(angle*PI/180.0) );
}

// zero_angle - used to zero out table angle, IE reset to zero.
void zero_angle(void)
{
    start_time = millis();  // save time we start our program
                            // to keep track of table rotation angle
}

// turns of rotation since table angle reset
int turns(void)
{
  return((millis()-start_time)/long(MS_PER_ROTATION));
}

// degrees - degrees of rotation since table angle reset
float angleD(void)
{
  return( float(millis()-start_time)/long(MS_PER_ROTATION)*360.0);
}

// radians of rotation since table angle reset
float angleR(void)
{
  return( float(millis()-start_time)/long(MS_PER_ROTATION)*2.0*PI );
}


// new position call for toothpick, then delay d
void set_servo(int p, unsigned int d)
{
   //p=constrain(p,-MAX_US,MAX_US);     // allow only negative max to max
   p=constrain(p,0,MAX_US);             // allowing negative works other side of plot, nothing lines up correctly
                                        // because of swing of the arm
   myservo.writeMicroseconds(p+1000);   // zero is a 1000us pulse
   if( d>0 ) delay(d);
}

// this sequence does a pretty clean smooth erase cycle
void erase() 
{ 
#if(ERASE)  
 zero_angle();    // reset table
 while(turns()<1)
 { 
  for(i = MAX_US; i >= 0; i -= 4)     // goes from 180 degrees to 0 degrees 
    set_servo(i,5);              // tell servo to go to position in variable 'pos' 
  for(i = 0; i <= MAX_US; i += 4) // goes from 0 degrees to 180 degrees 
    set_servo(i,5);              // tell servo to go to position in variable 'pos' 
 }
#endif
}

// home the toothpick to center of the table
void center()
{
  for(i = MAX_US; i >= 0; i -= 4)     // goes from 180 degrees to 0 degrees 
    set_servo(i,5);              // tell servo to go to position in variable 'pos' 
}

// home the toothpick to edge of the table
void rim()
{
  for(i = 0; i <= MAX_US; i += 4) // goes from 0 degrees to 180 degrees 
    set_servo(i,5);              // tell servo to go to position in variable 'pos' 
}

// showpage - delay while observers enjoy the page
void showpage(int min)
{
  while(min--)
     delay(60000);   // delay one minute
}

void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  set_servo( 0,5 );
  
  pinMode(2,INPUT_PULLUP);  // used to control what plots for makerfaire
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(8,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  pinMode(11,INPUT_PULLUP);
  
  //Serial.begin(9600);  // init serial port
  
  randomSeed(analogRead(0));  // seed random routine

  // call it a few times to jumble it
  for(i=1;i<random(10,20);i++)
    j=random(1,10);
} 


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX


// flower
//    radius decreases after each rotation
void flower(int lobes)
{
  erase();         // erase drawing
  center();
  zero_angle();    // reset table
  scale=1.0;
  
  for(i=1;i<=MAX_TURNS;i++)      // N turns
  {
    while(turns()<i)      // one turn, then change scale
    {
      set_servo( sin( angleR()*1.006*lobes ) * MAX_RAD * scale, SERVO_DELAY );
    }
    scale = scale * REDUCE_PER_TURN;
  }
} 

// make polygon  - for N sided polygon
//    transition to next inside polygon at start of each turn.
void polygon(int n)
{
  erase();         // erase drawing
  rim();
  zero_angle();    // reset table
  scale=1.0;
  float r=0.0;
  float pangle = 360/n;  // angle inside polygon side
  float tangle = 0.0;    // table angle
  
  for(i=1;i<=MAX_TURNS;i++)      // N turns
  {
    while(turns()<i)      // one turn, then change scale
    {
      tangle = (angleD()+(pangle/2))*1.005;    // compute angle within side of polygon
              // fudge to start in vertex, and enter in CCW twist fudge
      while(tangle>pangle) tangle=tangle-pangle;
      if(tangle<(pangle/2)) r=1/cosD(tangle);
      else                  r=1/cosD((pangle/2)-(tangle-(pangle/2)));

      // adjust r by polygon radius [factor = cos(pangle/2)], size of sand table, cycle reduction
      set_servo( r * cosD(pangle/2) * MAX_RAD * scale, SERVO_DELAY );
    }
    scale = scale * REDUCE_PER_TURN;    // was .95 adjust scale for next rotation of table
  }
}


// roof shingles - looks kinda like old postscript retina
//    radius continually decreasing
void roof_shingles()
{
  erase();         // erase drawing
  rim();
  zero_angle();    // reset table
  scale=1.0;
  int flowers = 40/2;  // 40 bumps about an inch apart
  int base = MAX_RAD-INCH_RAD;  // start about an inch from edge
  int mult = MAX_RAD-base;     // use full table
  
  while(turns()<MAX_TURNS)
  {
    // factor is CCW twist
    set_servo( (base+(mult* abs(cosD( angleD()*flowers*1.0125 )))) * scale, SERVO_DELAY );
    scale=scale*.999965;
  }
} 


// roof shinglesN - negative roof shingles looks kinda like old postscript retina
//    radius continually decreasing
void roof_shinglesN()
{
  erase();         // erase drawing
  rim();
  zero_angle();    // reset table
  scale=1.0;
  int flowers = 40/2;  // 40 bumps about an inch apart
  int base = MAX_RAD-INCH_RAD;  // start about an inch from edge
  int mult = MAX_RAD-base;     // use full table
  
  while(turns()<MAX_TURNS)
  {
    // factor is CCW twist
    set_servo( (base+(mult* (1.0-abs(cosD( angleD()*flowers*1.0125 ))))) * scale, SERVO_DELAY );
    scale=scale*.999965;
  }
} 

// make odd sided stars - like on flags
//    transition to next inside polygon at start of each turn.
void odd_star(int n)    // n=3,5,7,9 etc
{
  erase();         // erase drawing
  rim();
  zero_angle();    // reset table
  scale=1.0;
  float pangle = 360/n*(n/2);  // angle to next vertex drawn
  float tangle = 0.0;    // table angle
  
  for(i=n/2;i<=MAX_TURNS*(n/2);i=i+(n/2))      // N turns
  {
    while(turns()<i)      // one turn, then change scale
    {
      tangle = (angleD()+(pangle/2))*1.006;    // compute angle within side of polygon
              // fudge to start in vertex, and enter in CCW twist fudge
      while(tangle>pangle) tangle=tangle-pangle;
      if(tangle<(pangle/2)) r=1/cosD(tangle);
      else                  r=1/cosD((pangle/2)-(tangle-(pangle/2)));

      // adjust r by polygon radius [factor = cos(pangle/2)], size of sand table, cycle reduction
      set_servo( r * cosD(pangle/2) * MAX_RAD * scale, SERVO_DELAY );
    }
    scale = scale * REDUCE_PER_TURN;    // was .95 adjust scale for next rotation of table
  }
}


#ifdef UNDEF
// spirograph around circumference of a circle - Epicycloid(outside), Hypotrochoid(inside)
//  R=rad of large circle, r=rad of smaller circle, d= dist from
//  center of small circle for pen
//  Negative r values make the drawing inside - Hypotrochoid type.
//
//
//  For INSIDE spirographs-
//      Best to have R = MAX_RAD to use whole sand tray.
//      R-abs(r)+d <= MAX_RAD, so r=d is good.
//      d<=r or d>=R-abs(r)
//
//  For OUTSIDE spirographs-
//      R+abs(r)+d <= MAX_RAD
//      d<=r or d>=R+abs(r)
//
void spirograph(float R, float r, float d)
{
  erase();         // erase drawing
  rim();
  zero_angle();    // reset table
  scale=1.0;

  for(i=1;i<MAX_TURNS;i++)
  {
    while(turns()<i)      // N turns
    {
      float x=(R+r)*cos( angleR()) - d*cos((R+r)/r*angleR());
      float y=(R+r)*sin( angleR()) - d*sin((R+r)/r*angleR());
      float rad= sqrt(x*x + y*y);
      set_servo( rad * scale, SERVO_DELAY );
    }    
    scale = scale * REDUCE_PER_TURN;    // was .95 adjust scale for next rotation of table
  }
}
#endif


// make a nice spiral pattern
void spiral()
{
  erase();         // erase drawing
  rim();
  zero_angle();    // reset table
  scale=1.0;

  while(turns()<MAX_TURNS)      // N turns
  {
    for(i=MAX_RAD;i>MAX_RAD-50;i-=2)
      set_servo(i*scale,10);
    delay(200);
    for(i=MAX_RAD-50;i<MAX_RAD;i+=2)
      set_servo(i*scale,10);
    delay(200);  
    scale = scale * .9985;    // was .95 adjust scale for next rotation of table
  } 
}

// checkerboard
void checkerboard()
{
  erase();         // erase drawing
  rim();
  zero_angle();    // reset table
  scale=1.0;
  while(turns()<MAX_TURNS)
  {
    for(k=1;k<=10;k++)    // 20 cycles
    {
      for(j=1;j<=5;j++)  // 10 sweeps
      {
        for(i=MAX_RAD-50;i<=MAX_RAD;i+=1)  // sweep out
          set_servo( int(i*scale),20 );
        for(i=MAX_RAD;i>=MAX_RAD-50;i-=1)  // sweep in
          set_servo( int(i*scale),20 );
      }
      if(k!=10) delay(10000);
    }
    scale=scale*.80;
  }
}

// Cardioid plot
void cardioid()
{
  erase();         // erase drawing
  center();        // go to center
  zero_angle();    // reset table
  scale=1.0;

  for(i=1;i<MAX_TURNS;i++)
  {
    while(turns()<i)      // N turns
    {
      r = (MAX_RAD/2)*( 1-cos( angleR()) );    // polar cardioid equation
      set_servo( r * scale, SERVO_DELAY );
    }    
    scale = scale * REDUCE_PER_TURN;    // was .95 adjust scale for next rotation of table
  }
}


// Inverse Cardioid plot - parabola - goes off chart unless .5 coef inserted to make eliptical orbits
//     *3 makes three lobes.  Four might look like a clover leaf?
void inv_cardioid()
{
  erase();         // erase drawing
  rim();
  zero_angle();    // reset table
  scale=1.0;

  for(i=1;i<MAX_TURNS;i++)
  {
    while(turns()<i)      // N turns
    {
      r = (MAX_RAD/2)*( 1/(1-(.5*cos( angleR()*3))) );    // polar inv cardioid equation
      set_servo( r * scale, SERVO_DELAY );
    }    
    scale = scale * REDUCE_PER_TURN;    // was .95 adjust scale for next rotation of table
  }
}

// spiral erase - very time consuming - about an hour for yellow plates
void spiral_erase()
{
    r=0;
    center();
    zero_angle();    // reset table

    while(r<MAX_RAD)      // N turns till we reach edge
    {
      set_servo( r, SERVO_DELAY);
      r=angleR()*2;
    } 
}

// spiral erase fine - very time consuming - about 2 hours for yellow plates
void spiral_erase_fine()
{
    r=0;
    center();
    zero_angle();    // reset table

    while(r<MAX_RAD)      // N turns till we reach edge
    {
      set_servo( r, SERVO_DELAY);
      r=angleR()*1;
    } 
}

// nice donut plot
void donut()
{
  erase();         // erase drawing
  rim();
  zero_angle();    // reset table

  while(turns()<MAX_TURNS)      // N turns
  {
      r = (MAX_RAD-50)* abs(cos( angleR()*3.030 )) +50;
      set_servo( r * scale, SERVO_DELAY );
  }    
}

#ifdef UNDEF
// hyperbolic tile profile
static const short hyper_tab[61] = {
  286, 286, 286, 286, 287, 287, 288, 288, 289, 290,
  291, 292, 293, 294, 296, 297, 299, 301, 303, 305,
  307, 310, 312, 315, 318, 321, 324, 327, 331, 335,
  339, 343, 348, 353, 358, 363, 369, 375, 381, 388,
  396, 404, 412, 421, 431, 442, 453, 465, 478, 493,
  509, 528, 548, 569, 594, 624, 659, 700, 754, 832,
  1000 };
// table to simplify code below
static const int pow2[10] = { 0,1,2,4,8,16,32,64,128,256 };

// circular hyperbolic tile?
void hyper_tile()
{
  erase();         // erase drawing
  rim();
  zero_angle();    // reset table
  b=0.0;        // base circle diameter
  
  int n=3;        // sides
  float pangle = 360/n;  // angle to next vertex drawn
  float tangle = 0.0;    // table angle
  
  for(i=1;i<10;i++)      // N turns, depth of fractal drawing
  {
    b=MAX_RAD-(MAX_RAD/pow2[i]);
    while(turns()<i)      // one turn, then change scale
    {
      tangle = angleD()*pow2[i]+(pangle/2);    // compute angle within side of polygon
      while(tangle>pangle) tangle=tangle-pangle;
      if(tangle<(pangle/2)) r=hyper_tab[int(tangle)];
      else                  r=hyper_tab[int((pangle/2)-(tangle-(pangle/2)))];

      // adjust r by radius, size of sand table, cycle reduction
      set_servo( b+ (r / 1000.0 * MAX_RAD/pow2[i]), SERVO_DELAY );
    }
  }
}
#endif


// delicate shifting cosine wave
void shift_cos()
{
  erase();         // erase drawing
  rim();
  zero_angle();    // reset table
  scale=1.0;

  for(i=1;i<MAX_TURNS;i++)
  {
    while(turns()<i)      // N turns
    {
      r = (MAX_RAD*3/4)+ ((MAX_RAD/4)*cos( angleR()*10.06)*scale);    // polar inv cardioid equation
      set_servo( r, SERVO_DELAY );
    }    
    scale = scale * .8;    // was .95 adjust scale for next rotation of table
  }
}


#if MAKER
// makerfaire switch - use with 10K resistor to ground for each pin selected.
// Make a pattern then hang until reset with jumper moved.  Allows time for others
// to look and for me to do a manual erase.
void makerfaire()
{
  if(digitalRead(2)==LOW)
  {  flower(7); while(1){}; }
  
  if(digitalRead(3)==LOW)
  {  roof_shinglesN(); while(1){}; }
  
  if(digitalRead(4)==LOW)
  {  donut(); while(1){}; }
  
  if(digitalRead(5)==LOW)
  {  odd_star(5); while(1){}; }
  
  if(digitalRead(6)==LOW)
  {  polygon(random(3,8)); while(1){}; }
  
  if(digitalRead(7)==LOW)
  {  flower(random(3,12)); while(1){}; }

  if(digitalRead(8)==LOW)
  {  spiral_erase(); while(1){}; }

  if(digitalRead(10)==LOW)
  {  checkerboard(); while(1){}; }

  if(digitalRead(11)==LOW)
  {  shift_cos(); while(1){}; }
}
#endif

void loop() 
{ 
  #if MAKER
  makerfaire();
  #endif
  
  // what it should do when running at my house
  switch ( random(1,9) ){
  case 1: flower(random(3,9)); break;
  case 2: roof_shinglesN();    break;
  case 3: shift_cos();         break;
  case 4: polygon(random(3,7));break;
  case 5: odd_star(random(1,5)*2+1); break;
  case 6: spiral();            break;
  case 7: checkerboard();      break;
  case 8: spiral_erase();      break;
  }
} 
