#ifndef TYPE_ALIASER_H
#define TYPE_ALIASER_H

template <class tFrom, class tTo>
class TypeAliaser {
public:
	TypeAliaser(tFrom inFromValue) :
		mAsFromType(inFromValue) {  }
	tTo& Get() { return mAsToType; }


private:
	union {
		tFrom mAsFromType;
		tTo	  mAsToType;
	};
};
#endif
