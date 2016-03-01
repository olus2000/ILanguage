// Memory and general utilities
// Because the layout of types is subject to change, these functions
// should be the only ones used to access properties of V values.
// This does not apply to specific typed values (e.g. L) as these are
// more stable.

// In case changes are required in the future
#define MALLOC(l) malloc(l)
#define FREE(ptr) free(ptr)

// Declare a pointer type.
#define DECL(t, v) t v = MALLOC(sizeof(*v))
// Declare an array type; like (t v[l]) but on the heap.
#define DECL_ARR(t, v, l) t *v = MALLOC(sizeof(t)*(l))
// Declare a string of length l (not including null terminator).
#define DECL_STR(v, l) Str v = MALLOC((l)+1)
// Declare string of length l, and null-terminate it.
#define DECL_NSTR(v, l) DECL_STR(v, l); v[l]='\0'

I t_sizeof(T);   // Return the size of values of the input type.
I next_pow_2(I); // Return the smallest power of two >= the input.
// Test whether t is a pure power of two.
#define PURE(t)   (!((t)&((t)-1)))
// Test whether t is not a pure power of two.
#define IMPURE(t) ((t)&((t)-1))
// Ensure that the type of v is pure (modifies v).
#define PURIFY(v) while(IMPURE(T(v))) v=V(v)
// Like PURIFY, but deletes impure values.
// {PURIFY_D(v); ddel(v);} is identical to {ddel(v);}.
#define PURIFY_D(v) while(IMPURE(T(v))) { V vt=V(v); FREE(P(v)); v=vt; }
// Like PURIFY_D, but does not delete initial P(v).
// {PURIFY_D1(v); ddel(v);} is identical to {del(v);}.
#define PURIFY_D1(v) if(IMPURE(T(v))) { v=V(v); PURIFY_D(v) }

// v has type V.
// Return the type of v.
#define T(v) ((v).t)
// Return the pointer to the value of v.
#define P(v) ((v).p)
// Return the reference count of v. Segfaults if (!(T(v)&COMP_t)).
// Should probably not be used outside of mem.c.
#define REF(v) (**(I**)P(v))

// Put type and pointer together to construct an output of type V.
V TP(T, P);

// A constructor for each composite type.
O wrapO(V, I, V*);
F wrapF(V, I, V*);
L wrapL(T, I c, I l, I o, P);

// Functions (V newT(T t)) and (void setT(V v,T t)) for each type T
// newT creates a V value to wrap t.
// setT makes v's value equal to t, wrapping t is v has an impure type.
// In each case, t is consumed/reused.
// setT will silently cause bugs if (!(T(v)&T##_t)).
#define NEW(T) V new##T(T); void set##T(V,T);
ON_TYPES(ALL, NEW);
#undef NEW

// Some common cases of newT
L wrapArray(T t, I l, P p); // Create list with l elements p[i] of type t.
L wrapList(I l, V* v); // Create list with l V-type elements v.
L wrapStr(Str); // Turn a C string into an array of characters.
V makeStr(Str); // Same, but wraps into a V value.
V Err(Str); // Turn a C string into an error.

// Delete
void del(V);  // Delete this copy of v.
void ddel(V); // Delete this copy of v and free P(v).

// Move
// The first two arguments of each are destination and source.
void mv_P(V,V);  // Consumes value of source, but not pointer.
void mv_Pd(V,V); // Like mv_P, but frees pointer.
// cp_P and valcpy do not consume the source.
void cp_P(V,V);  // Like mv_P.
void valcpy(P,P,T); // P values must have the given type.

// Copy
V cpy(V);    // Return a new copy of the argument.
V cpy1(V);   // Copy only the pointer.
void get(V); // Ensure that the input is safely modifiable. Do not copy.


// For loop shortcuts
#define DO(var, max) for(var=0; var<max; var++)
#define DDO(var, max) I var; DO(var, max)

// Arithmetic utilities
// Arguments must not have side effects
#define max(a,b) ((a)>(b) ? (a):(b))
#define min(a,b) ((a)<(b) ? (a):(b))
#define floor(a)   ((a)==(Z)(a) ? a : (Z)(a) - ((a)<0))
#define ceiling(a) ((a)==(Z)(a) ? a : (Z)(a) + ((a)>0))
#define sign(a)  ((a)>0 ? 1 : -((a)<0))

// numbers
Z getZ(V);  // Input must have type Z.
R getR(V);  // Input must have type R or Z. Coerce to R.
// Get an Z value, taking floor or ceiling if value has type R.
#define getOPZ(vv, OP) ((T(vv))==Z_t ? Z(vv) : OP(R(vv)))
#define getFloorZ(v) getOPZ(v, floor)
#define getCeilZ(v) getOPZ(v,ceiling)

// lists
// l->t must be impure. Get the V value at index i.
#define LIST_AT(l, i) (((V*)(l)->p)[((i)+(l)->o)%(l)->c])
// Pointer for the value at index i, with no restriction on l.
#define LIST_PTR_AT(l, i) ((l)->p+t_sizeof((l)->t)*(((i)+(l)->o)%(l)->c))
// Like LIST_PTR_AT, but avoids recomputing t_sizeof is size s is known.
#define LIST_PTR_ATS(l, i, s) ((l)->p+(s)*(((i)+(l)->o)%(l)->c))
V list_at(L, I);      // Return the list value at the given index.
V list_ats(L, I, I);  // Same, if size (last argument) is known.
V listV_at(V, I);     // Like list_at for V argument
V listV_ats(V, I, I); // Like list_ats for V argument
