class ViewerAbstract {
    public:
        virtual void init(int w, int h) = 0;
        virtual void drawScene() = 0;
        virtual void updateAndDrawScene() = 0;
        virtual void reshape(int w, int h) = 0;
        virtual void loadShaders() = 0;

        // events
        virtual void mousePressed(int button, int action) = 0;
        virtual void mouseMoved(int x, int y) = 0;
        virtual void mouseScroll(double x, double y) = 0;
        virtual void keyPressed(int key, int action, int mods) = 0;
        virtual void charPressed(int key) = 0;
};