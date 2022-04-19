class TerrainGeneration {
    public:
        virtual void compile() = 0;
    private:
        unsigned int width;
        unsigned int height;
};