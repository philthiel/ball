// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/COMMON/exception.h>

using namespace::std;

namespace BALL 
{

	Residue::Residue()
		:	Fragment(),
			id_(BALL_RESIDUE_DEFAULT_ID),
			insertion_code_(BALL_RESIDUE_DEFAULT_INSERTION_CODE)
	{
	}
		
	Residue::Residue(const Residue& residue, bool deep)
		:	Fragment(residue, deep),
			id_(residue.id_),
			insertion_code_(residue.insertion_code_)
	{
	}
		
	Residue::Residue(const String& name, const String& id, char insertion_code)
		:	Fragment(name),
			id_(id),
			insertion_code_(insertion_code)
	{
	}

	Residue::~Residue()
	{
		destroy();
	}

	void Residue::clear()
	{
		Fragment::clear();
		id_ = BALL_RESIDUE_DEFAULT_ID;
		insertion_code_ = BALL_RESIDUE_DEFAULT_INSERTION_CODE;
	}
		
	void Residue::destroy()
	{
		Fragment::destroy();
		id_ = BALL_RESIDUE_DEFAULT_ID;
		insertion_code_ = BALL_RESIDUE_DEFAULT_INSERTION_CODE;

	}

	void Residue::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);
			Fragment::persistentWrite(pm);
			pm.writePrimitive(id_, "id_");
			pm.writePrimitive(insertion_code_, "insertion_code_");
		pm.writeObjectTrailer(name);
	}

	void Residue::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(RTTI::getStreamName<Fragment>());
			Fragment::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readPrimitive(id_, "id_");
		pm.readPrimitive(insertion_code_, "insertion_code_");
	}
		
	void Residue::set(const Residue& residue, bool deep)
	{
		Fragment::set(residue, deep);

		id_ = residue.id_;
		insertion_code_ = residue.insertion_code_;
	}
			
	Residue& Residue::operator = (const Residue& residue)
	{
		set(residue);
		return *this;
	}

	void Residue::get(Residue& residue, bool deep) const
	{
		residue.set(*this, deep);
	}
			
	void Residue::swap(Residue& residue)
	{
		Fragment::swap(residue);

		id_.swap(residue.id_);

		char temp_insertion_code = insertion_code_;
		insertion_code_ = residue.insertion_code_;
		residue.insertion_code_ = temp_insertion_code;
	}

	bool Residue::hasTorsionPsi() const
	{
		// instance must have a parent chain
		if (getChain() == 0)
		{
			return false;
		}
		// at least 2 residues are needed to create an angle
		if (getChain()->countResidues() < 2)
		{
			return false;
		}
		// the torsion angle psi is not defined for
		// the C-terminus
		return !isCTerminal() && hasProperty(PROPERTY__AMINO_ACID);
	}

	Angle Residue::getTorsionPsi() const
	{
		Angle result(0.0);
		if (hasTorsionPsi())
		{
			const Residue* next = getNext(RTTI::getDefault<Residue>());
			if (next != 0)
			{
				const Atom* C = 0;
				const Atom* N = 0;
				const Atom* CA = 0;
				AtomConstIterator it;
				for (it = beginAtom(); +it; ++it)
				{
					if (it->getName() == "C")	C  = &*it;
					if (it->getName() == "CA") CA = &*it;
					if (it->getName() == "N")	N  = &*it;
				}

				const Atom* next_N = 0;
				for (it = next->beginAtom(); +it; ++it)
				{
					if (it->getName() == "N")	
					{
						next_N  = &*it;
						break;
					}
				}
				
				if ((N != 0) && (C != 0) && (CA != 0) && (next_N != 0))
				{
					result = calculateTorsionAngle(*N, *CA, *C, *next_N);
				} 
				else
				{
					Log.error() << "Atoms not found:" << N << "/" << CA << "/" << C << "/" << next_N << " in residue " << getFullName() << " " << getID() << endl;
				}
			}
			else
			{
				Log.error() << "No next residue!" << endl;
			}
		}

		return result;
	}

	bool Residue::hasTorsionPhi() const
	{
		// instance must have a parent chain
		if (getChain() == 0)
		{
			return false;
		}
		// at least 2 residues are needed to create an angle
		if (getChain()->countResidues() < 2)
		{
			return false;
		}
		// the torsion angle phi is not defined for
		// the N-terminus
		return !isNTerminal() && hasProperty(PROPERTY__AMINO_ACID);
	}
	
	Angle Residue::getTorsionPhi() const
	{
		Angle result(0.0);
		if (hasTorsionPhi())
		{
			const Residue* previous = getPrevious(RTTI::getDefault<Residue>());
			if (previous != 0)
			{
				const Atom* C = 0;
				const Atom* N = 0;
				const Atom* CA = 0;
				AtomConstIterator it;
				for (it = beginAtom(); +it; ++it)
				{
					if (it->getName() == "C")	 C  = &*it;
					if (it->getName() == "CA") CA = &*it;
					if (it->getName() == "N")  N  = &*it;
				}

				const Atom* last_C = 0;
				for (it = previous->beginAtom(); +it; ++it)
				{
					if (it->getName() == "C")	
					{
						last_C  = &*it;
						break;
					}
				}

				if ((N != 0) && (C != 0) && (CA != 0) && (last_C != 0))
				{
					result = calculateTorsionAngle(*last_C, *N, *CA, *C);
				} 
				else
				{
					Log.error() << "Atoms not found:" << last_C << "/" << N << "/" << CA << "/" << C << " in residue " << getFullName() << " " << getID() << endl;
				}
			}
			else
			{
				Log.error() << "No previous residue!" << endl;
			}
		}

		return result;
	}

	bool Residue::hasTorsionOmega() const
	{
		// instance must have a parent chain
		if (getChain() == 0)
		{
			return false;
		}
		// at least 2 residues are needed to create an angle
		if (getChain()->countResidues() < 2)
		{
			return false;
		}
		// the torsion angle phi is not defined for
		// the N-terminus
		return  !isCTerminal() && hasProperty(PROPERTY__AMINO_ACID);
	}

	Angle Residue::getTorsionOmega() const
	{
		Angle result(0.0);
		if (hasTorsionOmega())
		{
			const Residue* next = getNext(RTTI::getDefault<Residue>());
			if (next != 0)
			{
				const Atom* C = 0;
				const Atom* CA = 0;
				AtomConstIterator it;
				for (it = beginAtom(); +it; ++it)
				{
					if (it->getName() == "C")  C  = &*it;
					if (it->getName() == "CA") CA = &*it;
				}

				const Atom* next_N = 0;
				const Atom* next_CA = 0;
				for (it = next->beginAtom(); +it; ++it)
				{
					if (it->getName() == "N")  next_N  = &*it;
					if (it->getName() == "CA") next_CA  = &*it;
				}

				if ((C != 0) && (CA != 0) && (next_N != 0) && (next_CA != 0))
				{
					result = calculateTorsionAngle(*CA, *C, *next_N, *next_CA);
				}
				else
				{
					Log.error() << "Atoms not found:" << CA << "/" << C << "/" << next_N << "/" << next_CA  << " in residue " << getFullName() << " " << getID() << endl;
				}
			}
			else
			{
				Log.error() << "No next residue!" << endl;
			}
		}

		return result;
	}

	Protein* Residue::getProtein()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor(); !ancestor_it.isEnd(); ++ancestor_it)
		{
            if (RTTI::isKindOf<Protein>(&*ancestor_it))
			{
				return (Protein *)&*ancestor_it;
			}
		}

		return 0;
	}

	const Protein* Residue::getProtein() const
	{
		return ((Residue *)this)->getProtein();
	}

	Chain* Residue::getChain()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor(); !ancestor_it.isEnd(); ++ancestor_it)
		{
            if (RTTI::isKindOf<Chain>(&*ancestor_it))
			{
				return (Chain *)&*ancestor_it;
			}
		}

		return 0;
	}

	const Chain *Residue::getChain() const
	{
		return ((Residue *)this)->getChain();
	}

	SecondaryStructure* Residue::getSecondaryStructure()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor(); !ancestor_it.isEnd(); ++ancestor_it)
		{
            if (RTTI::isKindOf<SecondaryStructure>(&*ancestor_it))
			{
				return (SecondaryStructure*)&*ancestor_it;
			}
		}

		return 0;
	}

	const SecondaryStructure* Residue::getSecondaryStructure() const
	{
		return const_cast<Residue*>(this)->getSecondaryStructure();
	}

	PDBAtom *Residue::getPDBAtom(Position position)
	{
		for (PDBAtomIterator protein_atom_iterator = beginPDBAtom();
				 !protein_atom_iterator.isEnd(); ++protein_atom_iterator)
		{
			if (position-- == 0)
			{
				return &(*protein_atom_iterator);
			}
		}

		return 0;
	}

	const PDBAtom* Residue::getPDBAtom(Position position) const
	{
		return ((Residue *)this)->getPDBAtom(position);
	}

	void Residue::setID(const String &id)
	{
		id_ = id;
	}

	const String &Residue::getID() const
	{
		return id_;
	}

	void Residue::setInsertionCode(char insertion_code)
	{
		// Is the insertion code a visible ASCII character
		if(insertion_code < 32 || insertion_code > 126) {
			throw Exception::InvalidArgument(__FILE__, __LINE__,
				"The specified insertion code is invalid. Only visible values "
				"and the space character ' ' are allowed."
			);
		}

		insertion_code_ = insertion_code;
	}

	void Residue::unsetInsertionCode() {
		insertion_code_ = ' ';
	}

	char Residue::getInsertionCode() const
	{
		return insertion_code_;
	}

	Size Residue::countPDBAtoms() const
	{
		Size size = 0;

		for (PDBAtomConstIterator protein_atom_iterator = beginPDBAtom();
				 !protein_atom_iterator.isEnd(); ++protein_atom_iterator)
		{
			++size;
		}

		return size;
	}

	void Residue::prepend(PDBAtom &protein_atom)
	{
		Fragment::prepend(protein_atom);
	}

	void Residue::append(PDBAtom& protein_atom)
	{
		Fragment::append(protein_atom);
	}

	void Residue::insert(PDBAtom& protein_atom)
	{
		Fragment::insert(protein_atom);
	}

	void Residue::insertBefore(PDBAtom& protein_atom, Composite& before)
	{
		Fragment::insertBefore(protein_atom, before);
	}

	void Residue::insertAfter(PDBAtom& protein_atom, Composite& after)
	{
		Fragment::insertAfter(protein_atom, after);
	}

	bool Residue::remove(PDBAtom& protein_atom)
	{
		return Fragment::remove(protein_atom);
	}

	void Residue::spliceBefore(Residue& residue)
	{
		Fragment::spliceBefore(residue);
	}

	void Residue::spliceAfter(Residue& residue)
	{
		Fragment::spliceAfter(residue);
	}

	void Residue::splice(Residue& residue)
	{
		Fragment::splice(residue);
	}

	bool Residue::isAminoAcid() const
	{
		return hasProperty(PROPERTY__AMINO_ACID);
	}

	bool Residue::isTerminal() const
	{
		return (isNTerminal() || isCTerminal());
	}

	bool Residue::isNTerminal() const
	{
		if (isAminoAcid() == true)
		{
			const Chain* chain = getChain();

			if (chain != 0)
			{
				ResidueConstIterator res_it = chain->beginResidue();
				for (; +res_it && &(*res_it) != this && !res_it->isAminoAcid(); ++res_it) {};

				return (&(*res_it) == this);
			}
		}

		return false;
	}
		
	bool Residue::isCTerminal() const
	{
		if (isAminoAcid() == true)
		{
			const Chain* chain = getChain();

			if (chain != 0)
			{
				ResidueConstIterator res_it(chain->endResidue());
				for (--res_it; +res_it && &(*res_it) != this && !res_it->isAminoAcid(); --res_it) {};
				return (&(*res_it) == this);
			}
		}

		return false;
	}
		
	bool Residue::isValid() const
	{ 
		return (Fragment::isValid() && id_.isValid());
	}

	void Residue::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		Fragment::dump(s, depth);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  id: " << id_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  insertion code: " << insertion_code_ << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	String Residue::getFullName(Residue::FullNameType type) const
	{
		// retrieve the residue name and remove blanks
		String full_name = getName();
		full_name.trim();

		// if the variant extension should be added, do so
		if (type == ADD_VARIANT_EXTENSIONS || type == ADD_VARIANT_EXTENSIONS_AND_ID)
		{
			String suffix = "-";
			if (isNTerminal()) 
			{	
				suffix = "-N";
			}
			if (isCTerminal()) 
			{
				suffix = "-C";
			}
			if (isCTerminal() && isNTerminal()) 
			{
				suffix = "-M";
			}
			if (hasProperty(Residue::PROPERTY__HAS_SSBOND)) 
			{
				suffix += "S";
			}
			
			if (suffix != "-")
			{
				full_name += suffix;
			}
		}
		if (type == ADD_RESIDUE_ID || type == ADD_VARIANT_EXTENSIONS_AND_ID)
		{
			full_name += String(getID());
		}

		return full_name;
	}

	bool Residue::operator == (const Residue& residue) const
	{
		return(
			Fragment::operator ==(residue)							&&
			id_							== residue.id_							&&
			insertion_code_	== residue.insertion_code_	);
	}

	bool Residue::operator != (const Residue& residue) const
	{
		return ! (*this == residue);
	}


} // namespace BALL
