/**************************************************************************
 * This file is property of and copyright by the ALICE HLT Project        *
 * All rights reserved.                                                   *
 *                                                                        *
 * Primary Authors:                                                       *
 *     Copyright 2009       Matthias Kretz <kretz@kde.org>                *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

#ifndef ALIHLTTPCCATRACKLETVECTOR_H
#define ALIHLTTPCCATRACKLETVECTOR_H

#include "AliHLTArray.h"
#include "AliHLTTPCCAParameters.h"
#include "AliHLTTPCCATrackParamVector.h"

#include "debug.h"

#ifndef NVALGRIND
#include <valgrind/memcheck.h>
#endif

class AliHLTTPCCATrackletVector
{
  public:
    AliHLTTPCCATrackletVector();

    uint_m IsValid() const { return fNHits > uint_v( Vc::Zero ); }

    uint_v NHits() const    { return fNHits;    }
    uint_v FirstRow() const { return fFirstRow; }
    uint_v LastRow() const  { return fLastRow;  }
    const AliHLTTPCCATrackParamVector &Param() const { return fParam; }

    unsigned int HitIndexAtRow( int rowIndex, int trackIndex ) const {
      return fRowHits[rowIndex][trackIndex];
    }
    uint_v HitIndexAtRow( int rowIndex ) const { return uint_v( &fRowHits[rowIndex][0] ); }

    void SetNHits   ( const uint_v &x ) { fNHits    = x; }
    void SetFirstRow( const uint_v &x ) { fFirstRow = x; }
    void SetLastRow ( const uint_v &x ) { fLastRow  = x; }
    void SetParam   ( const TrackParamVector &x ) { fParam = x; }

    void SetNHits   ( const uint_v &x, uint_m mask ) { fNHits(mask)    = x; }
    void SetFirstRow( const uint_v &x, uint_m mask ) { fFirstRow(mask) = x; }
    void SetLastRow ( const uint_v &x, uint_m mask  ) { fLastRow(mask)  = x; }
    void SetParam   ( const TrackParamVector &x, float_m mask  );

    void SetRowHits( int rowIndex, const uint_v &trackIndex, const uint_v &hitIndex );
    void SetRowHits( const uint_v &rowIndex, const uint_v &trackIndex, const uint_v &hitIndex );
    void SetRowHits( int rowIndex, const uint_v &trackIndex, const uint_v &hitIndex, const int_m &mask );
    void SetRowHits( const uint_v &rowIndex, const uint_v &trackIndex, const uint_v &hitIndex, const int_m &mask );

    void AddHitIds( const uint_v &rowIndexes, const uint_v &hitIndexes, const uint_m &mask );

    int MaxNRows() const { return fRowHits.size(); }
  private:
    uint_v fNHits;      // N hits
    uint_v fFirstRow;   // first TPC row
    uint_v fLastRow;    // last TPC row
    TrackParamVector fParam;   // tracklet parameters
    Vc::array< Vc::array<unsigned int, uint_v::Size>, AliHLTTPCCAParameters::MaxNumberOfRows8 > fRowHits; // hit index for each TPC row
};

inline void AliHLTTPCCATrackletVector::SetParam( const TrackParamVector &x, float_m mask )
{
  if( mask.isFull() ) {
    fParam = x;
    return;
  }
  else {
    fParam.SetTrackParam(x, mask);
  }
}

inline void AliHLTTPCCATrackletVector::SetRowHits( int rowIndex, const uint_v &trackIndex,
    const uint_v &hitIndex )
{
  assert( (trackIndex[0] + uint_v( Vc::IndexesFromZero ) == trackIndex).isFull() );
  assert( uint_m(( trackIndex[0] % uint_v::Size ) == 0).isFull() );
  UNUSED_PARAM1( trackIndex );
  VALGRIND_CHECK_VALUE_IS_DEFINED( hitIndex );
  VALGRIND_CHECK_MEM_IS_DEFINED( &fRowHits[rowIndex], sizeof( uint_v ) );
//  hitIndex.store( &fRowHits[rowIndex][0] );
  for( unsigned int i = 0; i < float_v::Size; i++ ) {
    fRowHits[rowIndex][i] = hitIndex[i];
  }
  VALGRIND_CHECK_MEM_IS_DEFINED( &fRowHits[rowIndex], sizeof( uint_v ) );
}

inline void AliHLTTPCCATrackletVector::SetRowHits( const uint_v &rowIndexes, const uint_v &trackIndex,
    const uint_v &hitIndex )
{
  assert( (trackIndex[0] + uint_v( Vc::IndexesFromZero ) == trackIndex).isFull() );
  assert( uint_m(( trackIndex[0] % uint_v::Size ) == 0).isFull() );
  UNUSED_PARAM1( trackIndex );
  VALGRIND_CHECK_VALUE_IS_DEFINED( hitIndex );
  std::cout<<"<!!!ERROR!!!> SetRowHits - Does not work now. Has to be changed or deleted\n";
}

inline void AliHLTTPCCATrackletVector::SetRowHits( int rowIndex, const uint_v &trackIndex,
    const uint_v &hitIndex, const int_m &mask )
{
  VALGRIND_CHECK_VALUE_IS_DEFINED( rowIndex );
  VALGRIND_CHECK_VALUE_IS_DEFINED( trackIndex );
  VALGRIND_CHECK_VALUE_IS_DEFINED( hitIndex );
  VALGRIND_CHECK_VALUE_IS_DEFINED( mask );
  assert( (trackIndex[0] + uint_v( Vc::IndexesFromZero ) == trackIndex).isFull() );
  assert( uint_m(( trackIndex[0] % uint_v::Size ) == 0).isFull() );
  UNUSED_PARAM1( trackIndex );
  assert( &fRowHits[0] != 0 );
  VALGRIND_CHECK_MEM_IS_DEFINED( &fRowHits[rowIndex], sizeof( uint_v ) );
//   debugF() << "TrackletVector::SetRowHits " << rowIndex << " old: ";
//   debugF() << uint_v( fRowHits[rowIndex] );
//   debugF() << " new: " << hitIndex << mask;
  if( mask.isEmpty() ) return;
  for( unsigned int i = 0; i < float_v::Size; i++ ) {
    if( !mask[i] ) continue;
    fRowHits[rowIndex][i] = hitIndex[i];
  }
//   debugF() << " done: " << uint_v( fRowHits[rowIndex] ) << std::endl;
  VALGRIND_CHECK_MEM_IS_DEFINED( &fRowHits[rowIndex], sizeof( uint_v ) );
}

inline void AliHLTTPCCATrackletVector::SetRowHits( const uint_v &rowIndexes, const uint_v &trackIndex,
    const uint_v &hitIndex, const int_m &mask )
{
  assert( (trackIndex[0] + uint_v( Vc::IndexesFromZero ) == trackIndex).isFull() );
  assert( uint_m(( trackIndex[0] % uint_v::Size ) == 0).isFull() );
  UNUSED_PARAM1( trackIndex );
  VALGRIND_CHECK_VALUE_IS_DEFINED( hitIndex );
  std::cout<<"<!!!ERROR!!!> SetRowHits - Does not work now. Has to be changed or deleted\n";
}

typedef AliHLTTPCCATrackletVector TrackletVector;

#endif // ALIHLTTPCCATRACKLETVECTOR_H
