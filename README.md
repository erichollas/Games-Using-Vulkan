StateMachine3DPong--
A game that interfaces with the VulkanRenderEngine for its graphics engine. The game is classical pong in 3D, implemented using State Machines and a Messanger object to send messages between said State Machines. (an exercise in state machines)

MultithreadedMinesweeper--
Another game that implements the same VulkanRenderEngine. Classical Minesweeper that features multithreading. The subsequent ReadMe will show where to find the examples of the multithreading. (an exercise in multithreading)

VulkanRenderEngine--
A render engine implemented using the Vulkan API. The goal for this is re-usability, which can be seen in the GeometryManager object. The GeometryManager manages the offsets of the vertices and indices of the various objects to be easily coordinated with the command buffer in the RenderEngine object, and serves as the main interface between game instance and render engine. (an exercies in graphics API's and writing re-usable programs)
