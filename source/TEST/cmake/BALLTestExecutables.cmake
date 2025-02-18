SET(BALL_BASIC_TESTS
	ClassTest_test
	Debug_test
	RegularExpression_test
	GlobalTypes_test
	Limits_test
	VersionInfo_test
	RTTI_test
)

SET(BALL_MATHS_TESTS
	MathsCommon_test
	AnalyticalGeometry_test
	Angle_test
	Vector2_test
	Vector3_test
	Vector4_test
	Quaternion_test
	Matrix44_test
	NumericalIntegrator_test
	Line3_test
	Circle3_test
	SimpleBox3_test
	Box3_test
	Plane3_test
	Sphere3_test
	Surface_test
	Function_test
	PiecewiseFunction_test
	PiecewisePolynomial_test
)
SET(BALL_SYSTEM_TESTS
	Directory_test
	FileSystem_test
	File_test
	Path_test
	PreciseTime_test
	Sysinfo_test
	Timer_test
	TimeStamp_test
	TransformationManager_test
)

SET(BALL_DATATYPE_TESTS
	Bit_test
	BitVector_test
	Contour_test
	ContourLine_test
	Hash_test
	HashMap_test
	StringHashMap_test
	HashSet_test
	HashGrid3_test
	HashGridBox3_test
	HashGrid3DataIteratorTraits_test
	HashGrid3BoxIteratorTraits_test
	HashGrid3DataItem_test
	HashGrid3NeighborBoxItem_test
	Options_test
	String_test1
	String_test2
	String_test3
	String_test4
	Substring_test
	Triple_test
	Quadruple_test
	RegularData1D_test
	RegularData2D_test
	RegularData3D_test
	ContourSurface_test
)

SET(BALL_CONCEPTS_TESTS
	LogStream_test
	AutoDeletable_test
	Factory_test
	Object_test
	PersistentObject_test
	PersistenceManager_test
	TextPersistenceManager_test
	Selectable_test
	NamedProperty_test
	PropertyManager_test
	Composite_test1
	Composite_test2
	Composite_test3
	Composite_test4
	CompositeCompositeIteratorTraits_test
	CompositeChildcompositeIteratorTraits_test
	CompositeAncestorIteratorTraits_test
	Embeddable_test
	BaseIterator_test
	ConstForwardIterator_test
	ForwardIterator_test
	ConstBidirectionalIterator_test
	BidirectionalIterator_test
	ConstRandomAccessIterator_test
	RandomAccessIterator_test
)

SET(BALL_DOCKING_TESTS
	IMGDock_test
	ConformationSet_test
	Conformation_test
	Constraints_test
	DockProblem_test
	DockResult_test
	DockingAlgorithm_test
	EvolutionaryDocking_test
	FlexDefinition_test
	FlexibleMolecule_test
	Genes_test
	GeneticAlgorithm_test
	GeneticIndividual_test
	GridAnalysis_test
	Parameter_test
	PoseClustering_test
	Receptor_test
	Result_test
	RotateBond_test
	RotateBonds_test
	SideChainOptimizer_test
	StaticLigandFragment_test
	StructurePreparer_test
)

IF(BALL_HAS_FFTW)
	LIST(APPEND BALL_DOCKING_TESTS
		GeometricFit_test
	)
ENDIF()

SET(BALL_SCORING_TESTS
	CHPI_test
	CHPISlick_test
	MMScoring_test
	PBScoring_test
	PB_test
	PLPScoring_test
	PLP_test
	AdvElectrostatic_test
	AmberEvaluation_test
	AromaticRingStacking_test
	BaseFunction_test
	BurialDepth_test
	BuriedPolar_test
	DiffGridBasedScoring_test
	DiffScoringFunction_test
	Electrostatic_test
	EnergeticEvaluation_test
	FermiBaseFunction_test
	ForceFieldEvaluation_test
	FragmentationalSolvation_test
	FresnoTypes_test
	GridBasedScoring_test
	GridedMM_test
	GridedPLP_test
	HydrogenBond_test
	HydrogenBondSlick_test
	LinearBaseFunction_test
	Lipophilic_test
	NonpolarSolvation_test
	Polarity_test
	PolarSolvation_test
	RandomEvaluation_test
	Rescorer_test
	Rescoring1D_test
	Rescoring3D_test
	Rescoring4D_test
	Rescoring_test
	RotationalEntropy_test
	ScoreGridSet_test
	ScoringComponent_test
	ScoringFunction_test
	ScoringOptimizer_test
	SlickEnergy_test
	SlickScore_test
	Solvation_test
	VanDerWaalsSlick_test
	VanDerWaals_test
)

SET(BALL_KERNEL_TESTS
	PTE_test
	Atom_test
	Bond_test
	AtomContainer_test
	Fragment_test
	Chain_test
	Molecule_test
	SecondaryStructure_test
	System_test
	Protein_test
	PDBAtom_test
	NucleicAcid_test
	Nucleotide_test
	Residue_test
	Expression_test
	ExpressionParser_test
	ExpressionPredicate_test
	ExpressionTree_test
	KernelPredicate_test
	Selector_test
	RuleEvaluator_test
	StandardPredicates_test
	DefaultProcessors_test
	FragmentDB_test
	NormalizeNamesProcessor_test
	ForEach_test
	GlobalKernel_test
	CompositeIteratorTraits_test
	AtomIterator_test
	AtomBondIteratorTraits_test
	Extractors_test
	StdIteratorWrapper_test
)

SET(BALL_MOLMEC_TESTS
	Gradient_test
	AtomVector_test
	MolmecSupport_test
	SnapShot_test
	SnapShotManager_test
	ForceField_test
	AmberFF_test
	MMFF94_test
	CharmmFF_test
	EnergyMinimizer_test
	LineSearch_test
	SteepestDescentMinimizer_test
	ConjugateGradientMinimizer_test
	StrangLBFGSMinimizer_test
	ShiftedLVMMMinimizer_test
	AtomTypes_test
)

SET(BALL_NMR_TESTS
	HBondProcessor_test
	RandomCoilShiftProcessor_test
	AnisotropyShiftProcessor_test
	ClearShiftProcessor_test
	CreateSpectrumProcessor_test
	EFShiftProcessor_test
	HaighMallionShiftProcessor_test
	JohnsonBoveyShiftProcessor_test
	ShiftModel_test
	ShiftModule_test
	Spectrum_test
	Peak_test
	PeakList_test
)

SET(BALL_FORMATS_TESTS
	AmiraMeshFile_test
	AntechamberFile_test
	LineBasedFile_test
	BinaryFileAdaptor_test
	Bruker1DFile_test
	Bruker2DFile_test
	CCP4File_test
	DockResultFile_test
	HMOFile_test
	INIFile_test
	JCAMPFile_test
	KCFFile_test
	Parameters_test
	ParameterSection_test
	ResourceFile_test
	GenericMolFile_test
	HINFile_test
	MOLFile_test
	SDFile_test
	MOL2File_test
	NMRStarFile_test
	DCDFile_test
	PDBRecords_test
	PDBInfo_test
	PDBFile_test
	TrajectoryFile_test
	XYZFile_test
	SCWRLRotamerFile_test
)

SET(BALL_ENERGY_TESTS
	AtomicContactEnergy_test
	ComposedEnergyProcessor_test
	EnergyProcessor_test
	OoiEnergy_test
)

SET(BALL_STRUCTURE_TESTS
	AddHydrogenProcessor_test
	AnalyticalSES_test
	AssignBondOrderProcessor_test
	AtomBijection_test
	BinaryFingerprintMethods_test
	BindingPocketProcessor_test
	ConnectedComponentsProcessor_test
	DisulfidBondProcessor_test
	Enumerator_test
	EnumeratorIndex_test
	GeometricProperties_test
	SimpleMolecularGraph_test
	NumericalSAS_test
	PeptideBuilder_test
	PeptideCapProcessor_test
	Peptides_test
	RadialDistributionFunction_test
	RDFIntegrator_test
	RDFParameter_test
	RDFSection_test
	ReconstructFragmentProcessor_test
	ResidueChecker_test
	RMSDMinimizer_test
	SideChainPlacementProcessor_test
	SmilesParser_test
	SmartsParser_test
	SmartsMatcher_test
	StructureMapper_test
	TransformationProcessor_test
	TranslationProcessor_test
	SurfaceProcessor_test
	SecondaryStructureProcessor_test
	UCK_test
	BuildBondsProcessor_test
#	MoleculeAssembler_test
#	SDGenerator_test
#	RingAnalyser_test
	ResidueRotamerSet_test
	RotamerLibrary_test
)

IF(BALL_HAS_OPENBABEL)
	LIST(APPEND BALL_STRUCTURE_TESTS
		OBinterface_test
	)
ENDIF()

SET(BALL_SOLVATION_TESTS
	PoissonBoltzmann_test
	SolventDescriptor_test
	SolventParameter_test
	ClaverieParameter_test
	ReissCavFreeEnergyProcessor_test
	UhligCavFreeEnergyProcessor_test
	PCMCavFreeEnergyProcessor_test
	PierottiCavFreeEnergyProcessor_test
	Pair6_12RDFIntegrator_test
	Pair6_12InteractionEnergyProcessor_test
	PairExpInteractionEnergyProcessor_test
	PairExpRDFIntegrator_test
	PARSE_test
)

SET(BALL_QSAR_TESTS
	AromaticityProcessor_test
	ConnectivityBase_test
	ConnectivityDescriptors_test
	Descriptor_test
	PartialChargeProcessor_test
	PartialChargeBase_test
	PartialChargeDescriptors_test
	RingPerceptionProcessor_test
	SimpleBase_test
	SimpleDescriptors_test
	SurfaceBase_test
	SurfaceDescriptors_test
	NBModel_test
	ALLModel_test
	FeatureSelection_test
	GPModel_test
	KNNModel_test
	KPCRModel_test
	KPLSModel_test
	LDAModel_test
	MLRModel_test
	PCRModel_test
	PLSModel_test
	RRModel_test
	SNBModel_test
)

SET(BALL_XDR_TESTS
	XDRPersistenceManager_test
)

SET(BALL_ASIO_TESTS
	Socket_test
)

### The list of all tests combined
SET(BALL_TESTS
	BALL_ASIO_TESTS
	BALL_BASIC_TESTS
	BALL_MATHS_TESTS
	BALL_SYSTEM_TESTS
	BALL_DATATYPE_TESTS
	BALL_CONCEPTS_TESTS
	BALL_DOCKING_TESTS
	BALL_SCORING_TESTS
	BALL_KERNEL_TESTS
	BALL_MOLMEC_TESTS
	BALL_NMR_TESTS
	BALL_FORMATS_TESTS
	BALL_ENERGY_TESTS
	BALL_STRUCTURE_TESTS
	BALL_SOLVATION_TESTS
	BALL_QSAR_TESTS
)

IF (BALL_HAS_XDR)
	LIST(APPEND BALL_TESTS BALL_XDR_TESTS)
ENDIF()
