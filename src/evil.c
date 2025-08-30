

typedef struct EvilClass EvilClass;

typedef struct EvilClass {
    int x;
    int y;
    int z;
    void(*method)(EvilClass*);
} EvilClass;


static void methodImpl(EvilClass* self);


EvilClass EvilClass_create() {
    return (EvilClass) {
        .x=0,
        .y=0,
        .z=0,
        .method=methodImpl,
    };
}

static void methodImpl(EvilClass* self) {
}


static void demo() {
    EvilClass class = EvilClass_create();

    class.method(&class);
}

