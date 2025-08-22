#ifndef SEALIB_LAZY_H
#define SEALIB_LAZY_H

#define Lazy(type) \
    struct  { \
        bool done; \
        type (^f)(); \
        type result; \
    }

#define Lazy_init(_f) { .f = _f, .done = false, }
#define Lazy_set(lazy_, _f) ({ (lazy_)->f = _f; (lazy_)->done = false; })
#define Lazy_get(lazy_) ( typeOf((lazy_)->result) ) ({ \
	if (!(lazy_)->done) { (lazy_)->result = (lazy_)->f(); (lazy_)->done = true; }  \
	(lazy_)->result; \
})
#define Lazy_copy(lazy_, arg) ({ memcpy((lazy_), (arg), sizeof(*(lazy_))); })

#endif //SEALIB_LAZY_H
