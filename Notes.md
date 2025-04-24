In OpenGL, objects like VAOs, VBOs, shaders, and textures are handles or
IDs that reference data stored in the GPU. So we use GLuint to store them.

lifecycle / pipeline of each object:
creation -> binding -> configuration -> usage -> unbinding / deletion.

Binding makes an object the active one in the context (window).
When we call a function, what it does depends on the internal state
of opengl - on the context/object. There can be only one active object
of each type at a time. fe: only one active VAO, VBO, texture, etc.
