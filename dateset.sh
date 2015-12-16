#!/bin/sh
echo '#ifndef __mDT_h' > $1/mDT.h
echo '#define __mDT_h' >> $1/mDT.h
echo -n '#define mHR ' >> $1/mDT.h
date +'%-H' >> $1/mDT.h
echo -n '#define mMN ' >> $1/mDT.h
date +'%-M' >> $1/mDT.h
echo -n '#define mS ' >> $1/mDT.h
date +'%-S' >> $1/mDT.h
echo -n '#define mMO ' >> $1/mDT.h
date +'%-m' >> $1/mDT.h >> $1/mDT.h
echo -n '#define mDY ' >> $1/mDT.h
date +'%-d' >> $1/mDT.h
echo -n '#define mYR ' >> $1/mDT.h
date +'%-Y' >> $1/mDT.h
echo -n '#define mWD ' >> $1/mDT.h
date +'%-w' >> $1/mDT.h
echo -n '#define mDOY ' >> $1/mDT.h
date +'%-j' >> $1/mDT.h
echo '#endif' >> $1/mDT.h

