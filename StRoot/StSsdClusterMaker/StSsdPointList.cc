#include "StSsdPointList.hh"

StSsdPointList::StSsdPointList()
{
  mListLength = 0;
  mFirstPoint = 0;
  mLastPoint  = 0;
}

StSsdPointList::~StSsdPointList()
{
  if (mListLength)
    {
      StSsdPoint *ptr = mLastPoint;
      StSsdPoint *toDele;
      while (mListLength)
	{
	  toDele = ptr;
	  ptr     = prev(ptr);
	  delete toDele;
	  mListLength--;
	}
    }
}

StSsdPoint* StSsdPointList::next(StSsdPoint *ptr)
{  return ptr->getNextPoint(); }

StSsdPoint* StSsdPointList::prev(StSsdPoint *ptr)
{  return ptr->getPrevPoint(); }

StSsdPoint* StSsdPointList::first()
{  return mFirstPoint; }

StSsdPoint* StSsdPointList::last()
{  return mLastPoint; }

int StSsdPointList::getSize()
{  return mListLength; }

int StSsdPointList::addNewPoint(StSsdPoint *ptr)
{
  if (!ptr) return 0;
  if (mListLength == 0)
    {
      ptr->setPrevPoint(0);
      ptr->setNextPoint(0);
      mFirstPoint = ptr;
      mLastPoint  = ptr;
    }
  else
    {
      ptr->setPrevPoint(mLastPoint);
      ptr->setNextPoint(0);
      mLastPoint->setNextPoint(ptr);
      mLastPoint = ptr;
    }
  mListLength++;
  return 1;
}

int StSsdPointList::removePoint(StSsdPoint *ptr)
{
  if (!this->getSize()) return 0;
  StSsdPoint *ptBefore = ptr->getPrevPoint();
  StSsdPoint *ptAfter  = ptr->getNextPoint();
  
  if (ptBefore == 0)
    {
      if (ptAfter== 0)
	{
	  // taille = 1
	  this->mFirstPoint =     0;
	  this->mLastPoint  =     0;
	  this->mListLength = 0;
	  delete ptr;
	  return 1;
	}
      else
	{
	  this->mFirstPoint = ptAfter;
	  ptAfter->setPrevPoint(0);
	  this->mListLength--;
	  delete ptr;
	  return 1;
	}
    }
  else
    {
      if (ptAfter== 0)
	{
	  this->mLastPoint = ptBefore;
	  ptBefore->setNextPoint(0);
	  this->mListLength--;
	  delete ptr;
	  return 1;
	}
      else
	{
	  ptBefore->setNextPoint(ptAfter);
	  ptAfter->setPrevPoint(ptBefore);
	  this->mListLength--;
	  delete ptr;
	  return 1;
	}
    }

}

void StSsdPointList::exchangeTwoPoints(StSsdPoint *ptr1,StSsdPoint *ptr2)
{
  int i = 0;

  ptr1->setFlag(ptr2->getFlag()) ;
  ptr1->setNPoint(ptr2->getNPoint()) ;
  ptr1->setNCluster(ptr2->getNCluster()) ;
  ptr1->setNMatched(ptr2->getNMatched());
  for (i = 0; i < 5; i++)
  ptr1->setNMchit(ptr2->getNMchit(i),i);
  ptr1->setNWafer(ptr2->getNWafer());
  for (i = 0; i < 3; i++)
   { 
  ptr1->setXg(ptr2->getXg(i),i);
  ptr1->setXl(ptr2->getXl(i),i);
   }
  for (i = 0; i < 2; i++)
   { 
  ptr1->setDe(ptr2->getDe(i),i);
   }

  ptr2->setFlag(ptr1->getFlag()) ;
  ptr2->setNPoint(ptr1->getNPoint()) ;
  ptr2->setNCluster(ptr1->getNCluster()) ;
  ptr2->setNMatched(ptr1->getNMatched());
  for (i = 0; i < 5; i++)
  ptr2->setNMchit(ptr1->getNMchit(i),i);
  ptr2->setNWafer(ptr1->getNWafer());
  for (i = 0; i < 3; i++)
   { 
  ptr2->setXg(ptr1->getXg(i),i);
  ptr2->setXl(ptr1->getXl(i),i);
   }
  for (i = 0; i < 2; i++)
   {
  ptr1->setDe(ptr2->getDe(i),i);
   }
}

StSsdPointList* StSsdPointList::addPointList(StSsdPointList *list)
{
  int size2 = list->getSize();
  
  if (!size2) return this;

  StSsdPoint *pt1 ;
  StSsdPoint *pt2 = list->first();
  
  for (int i = 0; i < size2; i++)
    {
      pt1 = pt2->giveCopy();
      this->addNewPoint(pt1);
      pt2 = list->next(pt2);
    }
  return this;  
}

StSsdPointList* StSsdPointList::substractPointList(StSsdPointList *list)
{

  int localSizeToDelete = list->getSize();
  int localSizeToKeep = this->getSize();

  if((!localSizeToDelete)||(!localSizeToKeep)) return this;
  StSsdPoint *currDele = list->first();

  for (int iDele = 0; iDele < localSizeToDelete; iDele++)
    {
      StSsdPoint *currKeep = this->first();
      int iKeep = 0;
      for (iKeep =0 ; ((iKeep < this->getSize())&&((currDele->getNPoint())!=(currKeep->getNPoint()))); iKeep++)
	{
	  currKeep = list->next(currKeep);
	}
      if (currDele->getNPoint()==currKeep->getNPoint()) 
	{
	  this->removePoint(currKeep);
	}
      currDele = list->next(currDele);
    }
  return this;
}

StSsdPointList* StSsdPointList::removeMultipleCount()
{
  int localSize = this->getSize();
  if (localSize < 2) return this;
  StSsdPoint *ptBigLoop = this->first();
  
  while ((ptBigLoop != this->last())&&(ptBigLoop != 0))
    {
      StSsdPoint *ptSmallLoop = this->next(ptBigLoop);

      while (ptSmallLoop!=0)
	{
	  if (ptSmallLoop->getNPoint() == ptBigLoop->getNPoint()) 
	    {
	      StSsdPoint *temp = ptSmallLoop;
	      ptSmallLoop = this->next(ptSmallLoop);
	      this->removePoint(temp);
	    }
	  else
	    {
	      ptSmallLoop = this->next(ptSmallLoop);
	    }
	}
      ptBigLoop = this->next(ptBigLoop);
    }
  return this;
}

StSsdPointList* StSsdPointList::sortPoint()
{
  int localSize=this->getSize();
  if (localSize<2) return this;
  
  StSsdPoint *ptCurr = this->first();
  ptCurr = this->next(ptCurr);
  for ( ; ptCurr!=0 ; )
    
    {
      StSsdPoint *ptB1 = ptCurr;
      StSsdPoint *ptB2;
      int isCont = 1;
      while ((ptB1 != this->first())&&(isCont))
	{
	  ptB2 = this->prev(ptB1);
	  if (ptB2->getNPoint() > ptB1->getNPoint())
	    {
	      this->exchangeTwoPoints(ptB1,ptB2);
	      ptB1 = ptB2;
	    }
	  else
	    {
	      isCont = 0;
	    }
	}
      ptCurr = this->next(ptCurr);
      
    }
  return this;
}

int StSsdPointList::renumHits(int last)
{
  int localSize = this->getSize();
  if (!localSize) return last;
  StSsdPoint *ptr = this->first();
  for (int iPt = 0; iPt < localSize; iPt++)
    {
      ptr->setNPoint(last + iPt + 1);
      ptr = this->next(ptr);
    }
  return (last + localSize);
}

StSsdPointList::StSsdPointList(const StSsdPointList & originalPointList)
{
  mListLength = originalPointList.mListLength;
  mFirstPoint     = originalPointList.mFirstPoint;
  mLastPoint      = originalPointList.mLastPoint;
}

StSsdPointList& StSsdPointList::operator=(const StSsdPointList originalPointList)
{
  mListLength = originalPointList.mListLength;
  mFirstPoint     = originalPointList.mFirstPoint;
  mLastPoint      = originalPointList.mLastPoint;

  return *this;
}
