# This is the repository for the code demo for teaching in parallel and distributed technologies

## Directory structure

    .
    ├── CL 
    │   ├── CommandQueueQuery  -  
    │   ├── ContextQuery  
    │   ├── DeviceQuery  
    │   ├── EuqueueKernel
    │   ├── Helloworld
    │   ├── KernelQuery
    │   ├── MatDotDiv
    │   ├── MatDotMul
    │   ├── MatMul_Native
    │   ├── MatMul_NativeVec
    │   ├── MatMul_Tiled
    │   ├── MatMul_TiledMore1D
    │   ├── MatMul_TiledMore2D
    │   ├── PlatformQuery
    │   ├── ProfileEvent
    │   ├── ProgramBuildQuery
    │   ├── ProgramQuery
    │   ├── VectorAdd
    │   └── VectorAdd1 
    │   
    ├── MPI
    │   ├── Allgather
    │   ├── Barrier
    │   ├── BlockRecv
    │   ├── Broadcast
    │   ├── BroadcastComp
    │   ├── BuffSend
    │   ├── CommManage
    │   ├── DeriveDataTypes
    │   ├── Gather
    │   ├── Gatherv
    │   ├── Group
    │   ├── HelloWorld
    │   ├── HelloWorld_simple
    │   ├── HelloWorld_sndrev
    │   ├── MatMul_Collective
    │   ├── MatMul_P2P
    │   ├── MatMul_P2P2
    │   ├── MatMul_P2P3
    │   ├── MsgRing
    │   ├── NonBlock
    │   ├── PackUnpack
    │   ├── PingPong
    │   ├── ReadySend
    │   ├── Reduce
    │   ├── Scatter
    │   ├── Scatterv
    │   ├── SendRecv
    │   ├── SyncSend
    │   ├── TopoCart
    │   ├── TopoGraph
    │   ├── TypeContig
    │   ├── TypeIndex
    │   ├── TypeStruct
    │   ├── TypeVec
    │   ├── VecAdd
    │   ├── VecAdd2
    │   ├── VecSum
    │   └── Wtime 
    │   
    ├── OMP 
    │   ├── AtomicVar
    │   ├── Barrier
    │   ├── BarrierOpt
    │   ├── ComputePI
    │   ├── CriticalOpt
    │   ├── CriticalRegion
    │   ├── DataClauses
    │   ├── FalseSharing
    │   ├── Flush
    │   ├── ForOpt
    │   ├── ForkJoin
    │   ├── HelloWorld
    │   ├── MatVecMul
    │   ├── ParaFor
    │   ├── ParaLoop
    │   ├── ParaRegion
    │   ├── Private
    │   ├── PrivateShared
    │   ├── ProfileTime
    │   ├── Reduction
    │   ├── ReductionSum
    │   ├── Runtime
    │   ├── Section
    │   ├── Shared
    │   ├── SharedMem
    │   ├── SingleOpt
    │   ├── Target
    │   ├── Task
    │   ├── ThreadNum
    │   ├── VecAdd
    │   └── omp_get_wtime 
    │   
    ├── macOS (Xcode + clang)
    │   ├── cl
    │   │   ├── printdevices
    │   │   └── test 
    │   ├── mpich
    │   │   └── test 
    │   └── omp 
    │       └── test 
    │   
    ├── win11
    │   ├── cl (Visual Studio 2019 + Intel OpenCL CPU Runtime)
    │   │   ├── printdevices
    │   │   └── test_ocl 
    │   ├── mpi (Visual Studio 2022 + Microsoft MPI)
    │   │   └── test_mpi 
    │   └── omp (Visual Studio 2022)
    │       └── test_omp
    │   
    ├── slides
    │   ├── CL_2021.pptx
    │   ├── MPI_2021.pptx
    │   └── MPI_2021.pptx 
    │   
    └── README.md

