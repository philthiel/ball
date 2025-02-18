// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// Molecular Mechanics Parameter: class describing parameters needed 
// for the EEF1 solvation component in the CHARMM force field
 
#ifndef BALL_MOLMEC_PARAMETER_CHARMMEEF1_H
#define BALL_MOLMEC_PARAMETER_CHARMMEEF1_H

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

namespace BALL 
{
	/**	Force Field Parameter Section for CHARMM EEF1.
			This class reads and administers a parameter set of solvation
			parameters for the CHARMM/EEF1 force field (Lazaridis, Karplus, ???).  \par
			armmEEF1.h
    	
			\ingroup  MolmecParameters
	*/
	class BALL_EXPORT CharmmEEF1 
		:	public ParameterSection
	{
		public:

		/**	?????
		*/
		struct Values 
		{
			/// ?????
			float V;
			/// ?????
			float dG_ref;
			/// ?????
			float dG_free;
			/// ?????
			float r_min;
			/// ?????
			float sig_w;
			/// ?????
			float dH_ref;
			/// ?????
			float Cp_ref;
		};

		/** ?????
		*/
		struct Data
		{
			/// ?????
			Atom*		atom;
			/// ?????
			Values	values;
		};


		/** @name Constructors and Destructor 
		*/
		//@{

		/**	Default constructor.  
		*/
		CharmmEEF1() ;

		/** Copy constructor 
		*/
		CharmmEEF1(const CharmmEEF1& charmm_EEF1) ;

		/**	Destructor.  
		*/
		virtual ~CharmmEEF1() ;

		/**	Clear method.  
		*/
		virtual void clear() ;

		//@}

		/**	Reads a parameter section from an INI file.
				This method reads the section given in section_name from ini_file,
				interprets (if given) a format line, reads the data from this
				section according to the format, and builds some datastructures for
				fast and easy acces this data.
		*/
		virtual bool extractSection(ForceFieldParameters& parameters, 
				const String& section_name) ;

		/** ?????
		*/
		virtual bool extractSection(Parameters& parameters, 
				const String& section_name) ;

		/** Queries whether a parameter set is defined for the given atom type.
		*/
		bool hasParameters(Atom::Type I) const ;
		
		/**	Returns the parameters for a given atom type combination.
		*/
		CharmmEEF1::Values getParameters(Atom::Type I) const ;
		
		/**	Assign the parameters for a given atom type combination.
				If no parameters are defined for this combination, false is
				returned and nothing is changed.
		*/
		bool assignParameters(CharmmEEF1::Values& parameters, Atom::Type I)
			const ;

		/** @name Assignment 
		*/
		//@{

		/** Assignment operator 
		*/
		const CharmmEEF1& operator = (const CharmmEEF1& charmm_EEF1) ;

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const CharmmEEF1& charmm_EEF1) const ;

		//@}

		protected:

		//_ ?????
		Size					number_of_atom_types_;

		//_ ?????
		float*				V_;
		
		//_ ?????
		float*				dG_ref_;
		
		//_ ?????
		float*				dG_free_;

		//_ ?????
		float*				dH_ref_;

		//_ ?????
		float*				Cp_ref_;

		//_ ?????
		float*				sig_w_;

		//_ ?????
		float*				R_min_;
		
		//_ ?????
		bool*					is_defined_;
	};
} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_CHARMMEEF1_H
