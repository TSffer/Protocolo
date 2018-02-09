
class Gamal
{
    private:
        ZZ d;
    public:
        string alfabeto = "0123456789,abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ-.";
        ZZ p;
        ZZ e1,e2;
        ZZ c1,km,c2;
        ZZ r;
        Gamal();
        Gamal(ZZ,ZZ,ZZ);
        void generarClaves(int);
        void darDatos();
        void encriptar(string);
        void desencriptar(string);

        bool esRP(ZZ g,ZZ n);
        ZZ RaizPrimitiva(ZZ p);
        ZZ RG(ZZ);
};