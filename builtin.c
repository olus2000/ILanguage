#define ON_ALL_NUMS(F) F(1) F(2) F(11) F(12) F(21) F(22)

#define D_D1(func) I func##_d1(V l)
#define D_D2(func) I func##_d2(V l, V r)
#define D_F1(func) V func##_f1(V l)
#define D_F2(func) V func##_f2(V l, V r)
typedef I (*D1)(V);     D1 B_d1[256];
typedef I (*D2)(V, V);  D2 B_d2[256];
typedef V (*F1)(V);     F1 B_f1[256];
typedef V (*F2)(V, V);  F2 B_f2[256];

#define D_D11(func) I func##_d11(V l, V ll)
#define D_D12(func) I func##_d12(V l, V ll, V rr)
#define D_D21(func) I func##_d21(V l, V r, V ll)
#define D_D22(func) I func##_d22(V l, V r, V ll, V rr)
#define D_F11(func) V func##_f11(V l, V ll)
#define D_F12(func) V func##_f12(V l, V ll, V rr)
#define D_F21(func) V func##_f21(V l, V r, V ll)
#define D_F22(func) V func##_f22(V l, V r, V ll, V rr)
typedef I (*D11)(V,V);      D11 B_d11[256];
typedef I (*D12)(V,V,V);    D12 B_d12[256];
typedef I (*D21)(V,V,V);    D21 B_d21[256];
typedef I (*D22)(V,V,V,V);  D22 B_d22[256];
typedef V (*F11)(V,V);      F11 B_f11[256];
typedef V (*F12)(V,V,V);    F12 B_f12[256];
typedef V (*F21)(V,V,V);    F21 B_f21[256];
typedef V (*F22)(V,V,V,V);  F22 B_f22[256];
#define EXTERN_BUILTINS  \
  extern F1 B_f1[256];   \
  extern F2 B_f2[256];   \
  extern D1 B_d1[256];   \
  extern D2 B_d2[256];   \
  extern D11 B_d11[256]; \
  extern D12 B_d12[256]; \
  extern D21 B_d21[256]; \
  extern D22 B_d22[256]; \
  extern F11 B_f11[256]; \
  extern F12 B_f12[256]; \
  extern F21 B_f21[256]; \
  extern F22 B_f22[256]

D_D1(true){return 1;}  D_D2(true){return 3;}
D_D11(true){return 1;}  D_D12(true){return 3;}
D_D21(true){return 1;}  D_D22(true){return 3;}

#include "compose.c"
#include "arith.c"
#include "compare.c"
#include "list.c"
void builtin_init() {
#define INIT(n) B_d##n[i]=&true_d##n; B_f##n[i]=NULL;
  DDO(i,256) { ON_ALL_NUMS(INIT) }
#undef INIT

  compose_init();
  arith_init();
  compare_init();
  list_init();
}

/////////////// Main definitions ///////////
V newF(B b, I n, V* x) {
  DECL_ARR(V,xx,n); DDO(i,n) xx[i]=x[i];
  DECL(B,bb); *bb=*b; return makeF(wrapB(bb),n,xx);
}

V apply_B1(B b, V* x) {
  F1 f=B_f1[*b]; if(!f) return newF(b,1,x);
  else {V v=f(x[0]); return v;}
}
V apply_B2(B b, V* x) {
  F2 f=B_f2[*b]; if(!f) return newF(b,2,x);
  else {V v=f(x[0],x[1]); return v;}
}
V apply_B(B b, I n, V* x) {
  switch (n) {
    case 1: return apply_B1(b,x);
    case 2: return apply_B2(b,x);
  }
}

I dom_B(B b, I n, V* x) {
  switch (n) {
    case 1: return B_d1[*b](x[0]);
    case 2: return B_d2[*b](x[0],x[1]);
  }
}

V apply_B11(B b, V* x, V* xx) {
  F11 f=B_f11[*b]; if(!f) return makeF(newF(b,1,x),1,xx);
  else {V v=f(cpy(x[0]), xx[0]); return v;}
}
V apply_B12(B b, V* x, V* xx) {
  F12 f=B_f12[*b]; if(!f) return makeF(newF(b,1,x),2,xx);
  else {V v=f(cpy(x[0]), xx[0], xx[1]); return v;}
}
V apply_B21(B b, V* x, V* xx) {
  F21 f=B_f21[*b]; if(!f) return makeF(newF(b,2,x),1,xx);
  else {V v=f(cpy(x[0]), cpy(x[1]), xx[0]); return v;}
}
V apply_B22(B b, V* x, V* xx) {
  F22 f=B_f22[*b]; if(!f) return makeF(newF(b,2,x),2,xx);
  else {V v=f(cpy(x[0]), cpy(x[1]), xx[0], xx[1]); return v;}
}
V apply_FB(F f, I n, V* xx) {
#define LINE(n) case n: return apply_B##n((B)f->f->v, f->x, xx);
  switch (10*f->l + n) { LINE(11) LINE(12) LINE(21) LINE(22) }
#undef LINE
}
