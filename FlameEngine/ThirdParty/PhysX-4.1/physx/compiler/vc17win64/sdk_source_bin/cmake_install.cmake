# Install script for directory: E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/compiler/cmake

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PhysX")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/source/foundation/include/windows" TYPE FILE FILES
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/windows/PsWindowsAoS.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/windows/PsWindowsFPU.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/windows/PsWindowsInclude.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/windows/PsWindowsInlineAoS.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/windows/PsWindowsIntrinsics.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/windows/PsWindowsTrigConstants.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/windows/PxWindowsIntrinsics.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/windows" TYPE FILE FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/windows/PxWindowsIntrinsics.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/unix/PxUnixIntrinsics.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/unix" TYPE FILE FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/unix/PxUnixIntrinsics.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXFoundation_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXFoundation_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXFoundation_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXFoundation_64.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxFoundation.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/foundation" TYPE FILE FILES
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/foundation/PxAssert.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/foundation/PxFoundationConfig.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/foundation/PxMathUtils.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/source/foundation/include" TYPE FILE FILES
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/Ps.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsAlignedMalloc.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsAlloca.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsAllocator.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsAoS.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsArray.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsAtomic.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsBasicTemplates.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsBitUtils.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsBroadcast.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsCpu.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsFoundation.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsFPU.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsHash.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsHashInternals.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsHashMap.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsHashSet.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsInlineAllocator.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsInlineAoS.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsInlineArray.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsIntrinsics.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsMathUtils.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsMutex.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsPool.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsSList.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsSocket.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsSort.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsSortInternals.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsString.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsSync.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsTempAllocator.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsThread.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsTime.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsUserAllocated.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsUtilities.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsVecMath.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsVecMathAoSScalar.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsVecMathAoSScalarInline.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsVecMathSSE.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsVecMathUtilities.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsVecQuat.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/foundation/include/PsVecTransform.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/Px.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxAllocatorCallback.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxProfiler.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxSharedAssert.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxBitAndData.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxBounds3.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxErrorCallback.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxErrors.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxFlags.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxIntrinsics.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxIO.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxMat33.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxMat44.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxMath.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxMemory.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxPlane.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxPreprocessor.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxQuat.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxSimpleTypes.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxStrideIterator.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxTransform.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxUnionCast.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxVec2.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxVec3.h;E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation/PxVec4.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/install/vc15win64/PxShared/include/foundation" TYPE FILE FILES
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/Px.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxAllocatorCallback.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxProfiler.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxSharedAssert.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxBitAndData.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxBounds3.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxErrorCallback.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxErrors.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxFlags.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxIntrinsics.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxIO.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxMat33.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxMat44.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxMath.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxMemory.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxPlane.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxPreprocessor.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxQuat.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxSimpleTypes.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxStrideIterator.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxTransform.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxUnionCast.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxVec2.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxVec3.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/../pxshared/include/foundation/PxVec4.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/gpu" TYPE FILE FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/gpu/PxGpu.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cudamanager" TYPE FILE FILES
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/cudamanager/PxCudaContextManager.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/cudamanager/PxCudaMemoryManager.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/common/windows" TYPE FILE FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/common/windows/PxWindowsDelayLoadHook.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysX_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysX_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysX_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysX_64.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxActor.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxAggregate.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxArticulationReducedCoordinate.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxArticulationBase.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxArticulation.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxArticulationJoint.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxArticulationJointReducedCoordinate.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxArticulationLink.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxBatchQuery.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxBatchQueryDesc.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxBroadPhase.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxClient.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxConstraint.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxConstraintDesc.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxContact.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxContactModifyCallback.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxDeletionListener.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxFiltering.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxForceMode.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxImmediateMode.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxLockedData.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxMaterial.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxPhysics.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxPhysicsAPI.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxPhysicsSerialization.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxPhysicsVersion.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxPhysXConfig.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxPruningStructure.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxQueryFiltering.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxQueryReport.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxRigidActor.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxRigidBody.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxRigidDynamic.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxRigidStatic.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxScene.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxSceneDesc.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxSceneLock.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxShape.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxSimulationEventCallback.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxSimulationStatistics.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxVisualizationParameter.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/common" TYPE FILE FILES
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/common/PxBase.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/common/PxCollection.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/common/PxCoreUtilityTypes.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/common/PxMetaData.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/common/PxMetaDataFlags.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/common/PxPhysicsInsertionCallback.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/common/PxPhysXCommonConfig.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/common/PxRenderBuffer.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/common/PxSerialFramework.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/common/PxSerializer.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/common/PxStringTable.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/common/PxTolerancesScale.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/common/PxTypeInfo.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/common/PxProfileZone.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/pvd" TYPE FILE FILES
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/pvd/PxPvdSceneClient.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/pvd/PxPvd.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/pvd/PxPvdTransport.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/collision" TYPE FILE FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/collision/PxCollisionDefs.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/solver" TYPE FILE FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/solver/PxSolverDefs.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/PxConfig.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXCharacterKinematic_static_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXCharacterKinematic_static_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXCharacterKinematic_static_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXCharacterKinematic_static_64.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/characterkinematic" TYPE FILE FILES
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/characterkinematic/PxBoxController.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/characterkinematic/PxCapsuleController.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/characterkinematic/PxController.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/characterkinematic/PxControllerBehavior.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/characterkinematic/PxControllerManager.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/characterkinematic/PxControllerObstacles.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/characterkinematic/PxExtended.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXCommon_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXCommon_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXCommon_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXCommon_64.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/geometry" TYPE FILE FILES
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxBoxGeometry.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxCapsuleGeometry.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxConvexMesh.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxConvexMeshGeometry.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxGeometry.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxGeometryHelpers.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxGeometryQuery.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxHeightField.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxHeightFieldDesc.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxHeightFieldFlag.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxHeightFieldGeometry.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxHeightFieldSample.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxMeshQuery.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxMeshScale.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxPlaneGeometry.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxSimpleTriangleMesh.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxSphereGeometry.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxTriangle.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxTriangleMesh.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxTriangleMeshGeometry.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geometry/PxBVHStructure.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/geomutils" TYPE FILE FILES
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geomutils/GuContactBuffer.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/geomutils/GuContactPoint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXCooking_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXCooking_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXCooking_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXCooking_64.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cooking" TYPE FILE FILES
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/cooking/PxBVH33MidphaseDesc.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/cooking/PxBVH34MidphaseDesc.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/cooking/Pxc.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/cooking/PxConvexMeshDesc.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/cooking/PxCooking.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/cooking/PxMidphaseDesc.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/cooking/PxTriangleMeshDesc.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/cooking/PxBVHStructureDesc.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXExtensions_static_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXExtensions_static_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXExtensions_static_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXExtensions_static_64.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/extensions" TYPE FILE FILES
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxBinaryConverter.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxBroadPhaseExt.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxCollectionExt.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxConstraintExt.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxContactJoint.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxConvexMeshExt.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxD6Joint.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxD6JointCreate.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxDefaultAllocator.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxDefaultCpuDispatcher.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxDefaultErrorCallback.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxDefaultSimulationFilterShader.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxDefaultStreams.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxDistanceJoint.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxContactJoint.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxExtensionsAPI.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxFixedJoint.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxJoint.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxJointLimit.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxMassProperties.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxPrismaticJoint.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxRaycastCCD.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxRepXSerializer.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxRepXSimpleType.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxRevoluteJoint.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxRigidActorExt.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxRigidBodyExt.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxSceneQueryExt.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxSerialization.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxShapeExt.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxSimpleFactory.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxSmoothNormals.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxSphericalJoint.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxStringTableExt.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/extensions/PxTriangleMeshExt.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/filebuf" TYPE FILE FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/filebuf/PxFileBuf.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXVehicle_static_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXVehicle_static_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXVehicle_static_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXVehicle_static_64.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/vehicle" TYPE FILE FILES
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleComponents.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleDrive.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleDrive4W.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleDriveNW.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleDriveTank.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleNoDrive.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleSDK.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleShaders.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleTireFriction.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleUpdate.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleUtil.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleUtilControl.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleUtilSetup.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleUtilTelemetry.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/vehicle/PxVehicleWheels.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/source/fastxml/include" TYPE FILE FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/source/fastxml/include/PsFastXml.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXPvdSDK_static_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXPvdSDK_static_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXPvdSDK_static_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXPvdSDK_static_64.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXTask_static_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXTask_static_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXTask_static_64.pdb")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE FILE OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXTask_static_64.pdb")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/task" TYPE FILE FILES
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/task/PxCpuDispatcher.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/task/PxTask.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/task/PxTaskDefine.h"
    "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/include/task/PxTaskManager.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXFoundation_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXFoundation_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXFoundation_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXFoundation_64.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXFoundation_64.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXFoundation_64.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXFoundation_64.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXFoundation_64.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysX_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysX_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysX_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysX_64.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysX_64.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysX_64.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysX_64.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysX_64.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXCharacterKinematic_static_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXCharacterKinematic_static_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXCharacterKinematic_static_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXCharacterKinematic_static_64.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXPvdSDK_static_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXPvdSDK_static_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXPvdSDK_static_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXPvdSDK_static_64.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXCommon_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXCommon_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXCommon_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXCommon_64.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXCommon_64.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXCommon_64.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXCommon_64.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXCommon_64.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXCooking_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXCooking_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXCooking_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXCooking_64.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXCooking_64.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXCooking_64.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXCooking_64.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE SHARED_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXCooking_64.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXExtensions_static_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXExtensions_static_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXExtensions_static_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXExtensions_static_64.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXVehicle_static_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXVehicle_static_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXVehicle_static_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXVehicle_static_64.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/debug" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/debug/PhysXTask_static_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Cc][Hh][Ee][Cc][Kk][Ee][Dd])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/checked" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/checked/PhysXTask_static_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Pp][Rr][Oo][Ff][Ii][Ll][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/profile" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/profile/PhysXTask_static_64.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/win.x86_64.vc142.md/release" TYPE STATIC_LIBRARY FILES "E:/Dev/FlameEngine/FlameEngine/ThirdParty/PhysX-4.1/physx/bin/win.x86_64.vc142.md/release/PhysXTask_static_64.lib")
  endif()
endif()

