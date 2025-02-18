// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularDynamics.C,v 1.22 2005/03/01 10:07:54 oliver Exp $
//

#include <BALL/MOLMEC/MDSIMULATION/molecularDynamics.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>
#include <BALL/KERNEL/PTE.h>

namespace BALL
{
	// Definition of default values and options
	const char* MolecularDynamics::Option::MAXIMAL_NUMBER_OF_ITERATIONS = "maximal_number_of_iterations";
  const char* MolecularDynamics::Option::MAXIMAL_SIMULATION_TIME = "maximal_simulation_time";
	const char* MolecularDynamics::Option::NUMBER_OF_ITERATION = "number_of_iteration";
	const char* MolecularDynamics::Option::ENERGY_OUTPUT_FREQUENCY = "energy_output_frequency";
	const char* MolecularDynamics::Option::SNAPSHOT_FREQUENCY = "snapshot_frequency";
	const char* MolecularDynamics::Option::TIME_STEP = "time_step";
	const char* MolecularDynamics::Option::REFERENCE_TEMPERATURE = "reference_temperature";
	const char* MolecularDynamics::Option::BATH_RELAXATION_TIME = "bath_relaxation_time";
	const char* MolecularDynamics::Option::CURRENT_TIME = "current_time";


	const Size MolecularDynamics::Default::MAXIMAL_NUMBER_OF_ITERATIONS = 10000;
  const double MolecularDynamics::Default::MAXIMAL_SIMULATION_TIME = 1;  // product of maxNoIterations and time step 
	const Size MolecularDynamics::Default::NUMBER_OF_ITERATION = 0;        // index of start iteration 
	const Size MolecularDynamics::Default::ENERGY_OUTPUT_FREQUENCY = 500;
	const Size MolecularDynamics::Default::SNAPSHOT_FREQUENCY = 500;
	const double MolecularDynamics::Default::TIME_STEP = 0.0001;
	const double MolecularDynamics::Default::REFERENCE_TEMPERATURE = 300.0;
	const double MolecularDynamics::Default::BATH_RELAXATION_TIME = 0.2;
	const double MolecularDynamics::Default::CURRENT_TIME = 0.0;          // start time 




	// The default constructor with no arguments
  MolecularDynamics::MolecularDynamics()
		:	valid_(false),
			force_field_ptr_(0),
			system_ptr_(0),
			atom_vector_(),
			time_step_(0.0),
			reference_temperature_(0.0),
			current_temperature_(0.0),			
			kinetic_energy_(0.0),
			total_energy_(0.0),
			current_time_(0.0),
			energy_output_frequency_(0),
			snapshot_frequency_(0),
			snapshot_manager_ptr_(0),
			abort_by_energy_enabled_(true),
			abort_energy_(1.e12)
	{
		// As no force field has been named, there is not much to do. 
		// Just indicate that the MD simulation is not ready yet. 
	}

	// Constructor expecting a force field 
	MolecularDynamics::MolecularDynamics(ForceField& force_field)
		:	system_ptr_(0),
			atom_vector_(),
			time_step_(0.0),
			reference_temperature_(0.0),
			current_temperature_(0.0),			
			kinetic_energy_(0.0),
			total_energy_(0.0),
			current_time_(0.0),
			energy_output_frequency_(0),
			snapshot_frequency_(0),
			snapshot_manager_ptr_(0),
			abort_by_energy_enabled_(true),
			abort_energy_(1.e12)
	{
		valid_ = true;
		force_field_ptr_ = &force_field;

		if (!valid_)
		{
			// The setup has failed for some reason. Output an error message.
			Log.error() << "Setup of instance of class 'MolecularDynamics' has failed." << std::endl;
		}
	}

	// Copy constructor 
	// Just for heaven's sake as it does standard memberwise copy and all classes used
	// overload the assignment operator on their own 
	MolecularDynamics::MolecularDynamics(const MolecularDynamics& rhs)
	{
		set(rhs);
	}

	// Destructor
	// Actually, nothing to do  here 
	MolecularDynamics::~MolecularDynamics()
	{
	}

	// The assignment operator 
	const MolecularDynamics& MolecularDynamics::operator = (const MolecularDynamics& rhs)
	{
		set(rhs);
		return *this;
	}
	
	void MolecularDynamics::set(const MolecularDynamics& rhs)
	{
		// Check if some fool tries a self--assignment. In that case, nothing is done
		// although it would not be a problem, anyway, as just memberwise copying is
		// done (i.e., this assignment operator corresponds to the default assignment)
		if (&rhs != this)
		{
			options = rhs.options;
			valid_ = rhs.valid_;
			force_field_ptr_ = rhs.force_field_ptr_;
			maximal_number_of_iterations_ = rhs.maximal_number_of_iterations_;
			number_of_iteration_ = rhs.number_of_iteration_;
			time_step_ = rhs.time_step_;
			reference_temperature_ = rhs.reference_temperature_;
			current_temperature_ = rhs.current_temperature_;
			current_time_ = rhs.current_time_;
			energy_output_frequency_ = rhs.energy_output_frequency_;
			snapshot_frequency_ = rhs.snapshot_frequency_;
			snapshot_manager_ptr_ = rhs.snapshot_manager_ptr_;
			abort_by_energy_enabled_ = rhs.abort_by_energy_enabled_;
			abort_energy_ = rhs.abort_energy_;
		}
	}


	// This  method tells whether the MD class is fully functional
	bool MolecularDynamics::isValid() const
	{
		return valid_;
	}


	// This method does all necessary initialisations. It basically calls
	// another setup method where the work is really done
	bool MolecularDynamics::setup(ForceField& force_field, SnapShotManager* ssm)
	{
		// No specific Options have been handed over, so we use the options of the
		// force field instead.
		valid_ = setup(force_field, ssm, force_field.options);

		return valid_;
	}

	void MolecularDynamics::enableEnergyAbortCondition(bool state)
	{
		abort_by_energy_enabled_ = state;
	}

	bool MolecularDynamics::energyAbortConditionEnabled() const
	{
		return abort_by_energy_enabled_;
	}

	void MolecularDynamics::setEnergyToAbort(float value)
	{
		abort_energy_ = value;
	}
		
	float MolecularDynamics::getEnergyToAbort() const
	{
		return abort_energy_;
	}
		

	// This is the real setup method doing all the work! 
	bool MolecularDynamics::setup(ForceField & force_field, SnapShotManager* ssm, const Options& new_options)
	{
		// First check whether the force field is valid. If not, then it is useless
		// to do anything here.
		if (force_field.isValid() == false)
		{
			// The setup has failed for some reason. Output an error message.
			Log.error() << "Setup of instance of class 'MolecularDynamics' has failed." << std::endl;
			Log.error() << "ForceField is not valid!" << std::endl;

			valid_ = false;
			return false;
		}

		// store the force field and some important data
		force_field_ptr_ = &force_field;
		system_ptr_ = force_field_ptr_->getSystem();

		// check if the user wants to do snapshots 
		if ((ssm != 0) && (ssm->isValid()))
		{
			snapshot_manager_ptr_ = ssm;
		}
		else
		{
			snapshot_manager_ptr_ = 0;
		}

		atom_vector_ = force_field_ptr_->getAtoms();

		// Check if the system is valid 
		if (system_ptr_ == 0)
		{
			Log.error() << "MolecularDynamics::setup: ERROR: No valid system bound to the force field.";
			valid_ = false;
			return false;
		}

		// set the options 
		options = new_options;

		// Set (if not already done) all class-specific options to their default values
		// and adopt them for internal class variables 

		// The number of iterations (= time steps) that will be carried out 
		options.setDefaultInteger (MolecularDynamics::Option::MAXIMAL_NUMBER_OF_ITERATIONS,
															 MolecularDynamics::Default::MAXIMAL_NUMBER_OF_ITERATIONS);
		maximal_number_of_iterations_ = (Size)options.getInteger (MolecularDynamics::Option::MAXIMAL_NUMBER_OF_ITERATIONS);

    // An equivalent formulation by time
		options.setDefaultReal(MolecularDynamics::Option::MAXIMAL_SIMULATION_TIME,     
													 MolecularDynamics::Default::MAXIMAL_SIMULATION_TIME);


		// The start value for the iteration number 
		options.setDefaultInteger(MolecularDynamics::Option::NUMBER_OF_ITERATION,
														  MolecularDynamics::Default::NUMBER_OF_ITERATION);
		number_of_iteration_ = (Size)options.getInteger(MolecularDynamics::Option::NUMBER_OF_ITERATION);


		// The length of a single time step in picoseconds 
		options.setDefaultReal (MolecularDynamics::Option::TIME_STEP, MolecularDynamics::Default::TIME_STEP);
		time_step_ = options.getReal(MolecularDynamics::Option::TIME_STEP);

		// The reference temperature. This temperature is important for MD runs with
		// heat-bath coupling enabled. 
		options.setDefaultReal(MolecularDynamics::Option::REFERENCE_TEMPERATURE,
													 MolecularDynamics::Default::REFERENCE_TEMPERATURE);
		reference_temperature_ = options.getReal(MolecularDynamics::Option::REFERENCE_TEMPERATURE);



		// The current time of the simulation. This is useful when several MD runs shall be done.
		options.setDefaultReal (MolecularDynamics::Option::CURRENT_TIME, MolecularDynamics::Default::CURRENT_TIME);
		current_time_ = options.getReal(MolecularDynamics::Option::CURRENT_TIME);

		// After how many iterations is energy data saved and the current kinetic
		// temperature calculated and saved 
		options.setDefaultInteger(MolecularDynamics::Option::ENERGY_OUTPUT_FREQUENCY,
															MolecularDynamics::Default::ENERGY_OUTPUT_FREQUENCY);
		energy_output_frequency_ = (Size)options.getInteger (MolecularDynamics::Option::ENERGY_OUTPUT_FREQUENCY);

		// After how many iterations are positions and velocities saved 
		options.setDefaultInteger (MolecularDynamics::Option::SNAPSHOT_FREQUENCY,
															 MolecularDynamics::Default::SNAPSHOT_FREQUENCY);

		snapshot_frequency_ = (Size)options.getInteger (MolecularDynamics::Option::SNAPSHOT_FREQUENCY);

		// Calculate the current temperature of the system (via kinetic energy)
		updateInstantaneousTemperature();

		valid_ = true;
		return true;
	}


	// This method allows us to set the current number of iteration for the MD simulation
  // The corresponding time is set as well. 
	void MolecularDynamics::setNumberOfIteration (Size number)
	{
		number_of_iteration_ = number;
    current_time_ = number * time_step_; 

    options[MolecularDynamics::Option::NUMBER_OF_ITERATION] = number;
    options[MolecularDynamics::Option::CURRENT_TIME] = current_time_; 
	}

	// This method allows us to get the current number of iteration for the MD simulation
	Size MolecularDynamics::getNumberOfIterations() const
	{
		return number_of_iteration_;
	}


	// This method sets the maximal number of iterations to be simulated 
	void MolecularDynamics::setMaximalNumberOfIterations (Size maximum)
	{
		maximal_number_of_iterations_ = maximum;

    options[MolecularDynamics::Option::MAXIMAL_NUMBER_OF_ITERATIONS] = maximum;
	}

	// This method gets the maximal number of iterations to be simulated 
	Size MolecularDynamics::getMaximalNumberOfIterations() const
	{
		return maximal_number_of_iterations_;
	}

	// This method sets the maximal simulation time in picoseconds. 
	void MolecularDynamics::setMaximalSimulationTime(double maximum)
	{
		maximal_number_of_iterations_ = (Size) (maximum / time_step_);

    options[MolecularDynamics::Option::MAXIMAL_NUMBER_OF_ITERATIONS] = maximal_number_of_iterations_; 

    double time = maximal_number_of_iterations_ * time_step_; 
    options[MolecularDynamics::Option::MAXIMAL_SIMULATION_TIME] = time; 
	}

	// This method gets the maximal simulation time                        
	double MolecularDynamics::getMaximalSimulationTime() const
	{
		return maximal_number_of_iterations_ * time_step_;
	}

  // This method sets the time step for the numerical integration
  void MolecularDynamics::setTimeStep(double step)
  {
	  if (step > 0)
		{
			time_step_ = step; 
			options[MolecularDynamics::Option::TIME_STEP] = time_step_; 
		}
		else
	  {
			Log.warn() << "Assigning a time step of zero is not allowed. "
								 << "Using old value." << std::endl;
		}
  }

  // This method gets the time step for the numerical integration
  double MolecularDynamics::getTimeStep() const 
  {
	  return time_step_; 
  }


	// This method allows us to reset the reference temperature 
	// without doing a full setup again 
	void MolecularDynamics::setReferenceTemperature (double temperature)
	{
		reference_temperature_ = temperature;

    options[MolecularDynamics::Option::REFERENCE_TEMPERATURE] = reference_temperature_; 
	}

	// This methods resets the current time 
  // The difference between the maximal simulation time and current time is
  // being simulated. 
	void MolecularDynamics::setCurrentTime (double time)
	{
		current_time_ = time;
		number_of_iteration_ = (Size) (time / time_step_);

    options[MolecularDynamics::Option::CURRENT_TIME] = current_time_; 
    options[MolecularDynamics::Option::NUMBER_OF_ITERATION] = number_of_iteration_; 
	}

	// This method allows us to reset the output frequency for 
	// energy values
	void MolecularDynamics::setEnergyOutputFrequency (Size number)
	{
		if (number > 0)
    {
			energy_output_frequency_ = number;
      options[MolecularDynamics::Option::ENERGY_OUTPUT_FREQUENCY] = number; 
    }
		else
		{
			Log.warn() << "Assigning an energy_output_frequency of zero is not allowed. Using old value." << std::endl;
		}
	}


	// This method allows us to reset the frequency of taking snapshots 
	// of the system
	void MolecularDynamics::setSnapShotFrequency (Size number)
	{
		if (number > 0)
		{
			snapshot_frequency_ = number;
      options[MolecularDynamics::Option::SNAPSHOT_FREQUENCY] = snapshot_frequency_; 
		}
		else
		{
			Log.warn() << "Assigning a snapshot_frequency_ of zero is not allowed. "
								 << "Using old value." << std::endl;
		}
	}

	// Obtain the current frequency for energy output (if the user has
	// forgotten it or what?)
	Size MolecularDynamics::getEnergyOutputFrequency() const
	{
		return energy_output_frequency_;
	}

	// Obtain the current frequency for trajectory output (if the user has
	// forgotten it or what?)
	Size MolecularDynamics::getSnapShotFrequency() const
	{
		return snapshot_frequency_;
	}

	// This method  returns the current (kinetic) temperature of the system
	double MolecularDynamics::getTemperature() const
	{
		return current_temperature_;
	}


	// This method returns the current potential energy
	double MolecularDynamics::getPotentialEnergy() const
	{
		return force_field_ptr_->getEnergy();
	}

	// This method returns the current kinetic energy
	double MolecularDynamics::getKineticEnergy() const
	{
		return kinetic_energy_;
	}

	// This method returns the current total energy
	double MolecularDynamics::getTotalEnergy() const
	{
		return total_energy_;
	}

	// This method returns the current time of the MD run
	double MolecularDynamics::getTime() const
	{
		return current_time_;
	}

	// This method returns the force field the system is bound to 
	ForceField *MolecularDynamics::getForceField() const
	{
		return force_field_ptr_;
	}

	// This method will be overwritten by a subclass
	bool MolecularDynamics::simulateIterations(Size /* number */, bool /* restart */)
	{
		return true;
	}

	// This method does the actual simulation stuff
  // It runs for getMaximalNumberOfIterations() iterations. 
  // restart=true means that the counting of iterations is started with the end
  // value of the previous run
	bool MolecularDynamics::simulate(bool restart)
	{
		return simulateIterations(maximal_number_of_iterations_, restart);
	}


	// This method does the actual simulation stuff
	// It runs for the indicated simulation time in picoseconds. 
  // restart=true means that the counting of iterations is started with the end
  // value of the previous run
	bool MolecularDynamics::simulateTime(double simulation_time, bool restart)
	{
		Size number;

		// determine the number  of iterations and call 'simulateIterations'
		if (valid_)
		{
			number = static_cast<Size>(simulation_time / time_step_);
			if (!simulateIterations (number, restart)) return false;
		}

		return true;
	}

	// This method will be overwritten by a subclass
	bool MolecularDynamics::specificSetup()
	{
		return true;
	}

	// This method calculates the instantaneous temperature (from velocities)
	// and thereby also the kinetic energy 
	void MolecularDynamics::updateInstantaneousTemperature()
	{
		// First define some local variables 
		double sq_velocity;
		double sum;

	  vector <Atom*>::iterator atom_it;

	  sum = 0.0;

		// The current temperature (calculated as instantaneous kinetic energy)
		// If we use a periodic boundary box, then we use the molecules' centres of
		// gravity, otherwise we iterate directly over the individual atoms of the system 
	  atom_it = atom_vector_.begin();

		if (force_field_ptr_->periodic_boundary.isEnabled() == true)
		{
			double molecule_mass = 0;
			float mass;

			Size no_of_molecules = 0;

			Molecule* old = 0;
			Molecule* current = 0;
			Vector3 centre_velocity;


		  centre_velocity.x = centre_velocity.y = centre_velocity.z = 0;

			// Get the molecule of the first atom in the system 
			if (atom_it != atom_vector_.end())
			{
				old = (*atom_it)->getMolecule();
			}

			// Iterate over all atoms in the system and determine for every
			// molecule the velocity of its centre of mass
			// This velocity contributes to the overall temperature 
		  vector<Atom*>::iterator end_it = atom_vector_.end();

			while (atom_it != end_it)
			{
				// determine the molecule of the current atom
				current = (*atom_it)->getMolecule();

				if (current != old)
				{
					// This atom belongs to an other molecule than the 
					// previous atom -> all atoms of the previous 
					// molecule have been visited
					centre_velocity /= molecule_mass;
					sq_velocity = centre_velocity * centre_velocity;
					sum += molecule_mass * sq_velocity;

					no_of_molecules++;

					// reset some values for the new molecule
					molecule_mass = 0;
					centre_velocity.x = centre_velocity.y = centre_velocity.z = 0;

					// the new molecule is now the old one
					old = current;
				}

				// add the atom's velocity to the total velocity  
				// of the current molecule
				mass = (*atom_it)->getElement().getAtomicWeight();
				molecule_mass += mass;
				centre_velocity += mass * (*atom_it)->getVelocity();


				// go on to the next atom
				++atom_it;
			}

			// The last molecule must be added as this is not done in the while-loop
			if (molecule_mass == 0)
			{
				// A strange molecule. Let the centre_velocity as it is
			}
			else
			{
				centre_velocity /= molecule_mass;
			}

			sq_velocity = centre_velocity * centre_velocity;
			sum += molecule_mass * sq_velocity;
			no_of_molecules++;

			// The factor 0.01 transfers Da * A^2/ps^2 to kJ/mol 
			kinetic_energy_ = 0.01 * 0.5 * sum;

			// We are now ready to calculate the instantaneous kinetic
			// temperature 
			// T = 2 * E_kin / (3 * #molecules * k_B)
			// The factor 1e3 / AVOGADRO transforms it into K
			current_temperature_ = 1e3 / Constants::AVOGADRO * 2 *
			kinetic_energy_ / (3.0 * no_of_molecules * Constants::BOLTZMANN);
		}
		else
		{
			// No periodic boundary conditions, i.e., just a few molecules 
			// are in the system
			Size no_of_atoms = 0;

			// Iterate over all atoms and calculate \sum m_i \cdot v_i^2  
			for (atom_it = atom_vector_.begin(); atom_it != atom_vector_.end(); ++atom_it)
			{
				no_of_atoms++;
				sq_velocity = (*atom_it)->getVelocity() * (*atom_it)->getVelocity();
				sum += (*atom_it)->getElement().getAtomicWeight() * sq_velocity;

			}

			// The factor 0.01 transfers Da * A^2/ps^2 to kJ/mol 
			kinetic_energy_ = 0.01 * 0.5 * sum;

			if (no_of_atoms == 0)
			{
				// a system with no atoms?!
				// do nothing with the temperature 
				current_temperature_ = 0;
			}
			else
			{
				// T = 2 * E_kin / (3 * #atoms * k_B)
				// multiply by 1/(3 * n * k_B) 
				// The factor 1e3 / Constants::AVOGADRO transforms it into K
				current_temperature_ = 1e3 / Constants::AVOGADRO * 2 *
					kinetic_energy_ / (3.0 * no_of_atoms * Constants::BOLTZMANN);
			}

		}

		// Determine the total energy of the selected atoms 
		total_energy_ = kinetic_energy_ + force_field_ptr_->getEnergy();

	}	// end of 'updateInstantaneousTemperature' 

}	// end of namespace BALL   
