#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define DT              1.0e-2
#define OUTDT           1.0e-1
#define TMAX            5.0e+1
#define MAXITER         ((int)((TMAX)/(DT)))
#define OUTGAP          ((int)((OUTDT)/(DT)))

#define L               10.0  // domain size: LxL
#define NUM_PARTICLE    1000  // number of particles
#define V0              1.0   // particle velocity
#define ETA             0.1   // noise level 
#define R               0.2   // radius

double randRange(double min, double max){return (rand()/(RAND_MAX/(max - min)) + min);}
double randBasic(){return (rand()/(double)(RAND_MAX));}

/* class */
class Particle
{
  public:
    Particle();
    ~Particle();

  // member functions (method)
  public:
    void    Update();
    void    CalcVelocity();
    void    Output(int iter);

  // member variables
  private:
    double  *x_, *v_, *theta_, *thetaN_;
    int     numParticle_;
};

/* constructor */
Particle::Particle()
{
  numParticle_ = NUM_PARTICLE; 

  // allocate variables: multiply 2 for 2D simulation
  x_      = new double[2*numParticle_];
  v_      = new double[2*numParticle_];
  theta_  = new double[  numParticle_];
  thetaN_ = new double[  numParticle_];

  srand(0); // set seed for random generator
  for(int idParticle = 0; idParticle < numParticle_; idParticle++){
    // set initial position 
    x_[2*idParticle + 0] = randRange(-L/2.0, +L/2.0);
    x_[2*idParticle + 1] = randRange(-L/2.0, +L/2.0);

    // set initial velocity
    theta_[idParticle] = 2.0*M_PI*randRange(0.0, 1.0);
    v_[2*idParticle + 0] = V0*cos(theta_[idParticle]);
    v_[2*idParticle + 1] = V0*sin(theta_[idParticle]);
  }
}

/* destructor */
Particle::~Particle()
{
  // free variables
  delete [] x_;
  delete [] v_;
}

void Particle::CalcVelocity()
{
  for(int i = 0; i < numParticle_; i++){
    double tvx = cos(theta_[i]);
    double tvy = sin(theta_[i]);

    /* Step 3: particle interactions - first term of Eq.(3) */
    /*
    for(int j = 0; j < numParticle_; j++){
      if(i == j) continue;

      // Step 3: compute distance r between two particles: i and j
      double  rx = ...;
      double  ry = ...;

      // Step 4: Apply periodic boundary condition to the particle interactions

      double  r  = ...;

      // add velocity if the particle j is inside the  distance R
      if(r < R){
        tvx += cos(theta_[j]);
        tvy += sin(theta_[j]);
      }
    }
    */

    /* Step 5: add whitenoise to the particle orientation (use MACRO value, ETA) */
    thetaN_[i] = atan2(tvy, tvx);

    v_[2*i + 0] = V0*cos(thetaN_[i]);
    v_[2*i + 1] = V0*sin(thetaN_[i]);
  }
}

void Particle::Update()
{
  for(int idParticle = 0; idParticle < numParticle_; idParticle++){
    double tx = x_[2*idParticle + 0] + v_[2*idParticle + 0]*DT;
    double ty = x_[2*idParticle + 1] + v_[2*idParticle + 1]*DT;

    /* Step 2: periodic boundary condition */
    /*
    if(tx > +L/2.0) ...;
    ...
    */

    // update position
    x_[2*idParticle + 0] = tx;
    x_[2*idParticle + 1] = ty;

    // update orientation
    theta_[idParticle] = thetaN_[idParticle];
  }
}

void Particle::Output(int iter)
{
  if(iter%OUTGAP != 0) return;

  FILE *fp;
  char filename[256];

  sprintf(filename, "result/result%05d.dat", iter/OUTGAP);
  if((fp = fopen(filename, "w")) == NULL){printf("FAILED TO OPEN FILE.\n"); exit(1);};
  for(int idParticle = 0; idParticle < numParticle_; idParticle++)
    fprintf(fp, "%15e %15e %15e %15e\n",
      x_[2*idParticle + 0], x_[2*idParticle + 1],
      v_[2*idParticle + 0], v_[2*idParticle + 1]);
  fclose(fp);


  /* Step 7: evaluate polar-order-parameter */
  /*
  double tvx = 0.0, tvy = 0.0;
  for(int idParticle = 0; idParticle < numParticle_; idParticle++){
    // compute average 
    tvx += ...;
    tvy += ...;
  }

  // output polar-order-parameter
  sprintf(filename, "result/eta%.2e.dat", ETA);
  if((fp = fopen(filename, "a")) == NULL){printf("FAILED TO OPEN FILE.\n"); exit(1);};
  fprintf(fp, "%15e %15e\n", iter*DT, ...);
  fclose(fp);
  */
}

/* main function */
int main()
{
  Particle particle;

  // main loop - iteration
  for(int iter = 0; iter <= MAXITER; iter++){
    particle.Output(iter);    // output result

    particle.CalcVelocity();  // calculate particle velocity
    particle.Update();        // update particle position
  }

  return 0;
}
