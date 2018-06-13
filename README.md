StateMachine3DPong--
A game that interfaces with the VulkanRenderEngine for its graphics engine. The game is classical pong in 3D implemented using State Machines and a Messanger object to send messages between said State Machines. (Maybe not the best way to do a pong game, but an exercise in state machines.)

MultithreadedMinesweeper--
Another game that interfaces with the same VulkanRenderEngine. Classical Minesweeper that uses mutlithreaded algorithms.
(coming soon....)

VulkanRenderEngine--
A renderring engine implemented using the Vulkan API. The goal for this is re-usability, which can be seen in the GeometryManager object. The GeometryManager manages the offsets of the vertices and indices of the various objects to be easily drawn in the command buffer in the RenderEngine object, and serves as the main interface between game instance and render engine.
