# Neural_Network
designed an operating system that implements a neural network architecture using separate
processes and threads on a multi-core processor. The system uses inter-process communication
through pipes for exchanging information such as weights and biases between processes. Each layer of
the neural network is represented as a separate process, and each neuron within a layer is
treated as a separate thread. During backpropagation, the error signal is propagated backward
through the layers of the network, and the system updates the weights and biases based on the
calculated gradients, while utilizing the processing power of multiple cores.

The operating system provides a mechanism for inter-process communication (IPC) through pipes
to exchange information between processes. Weights and biases, which are the parameters that the
neural network learns during training, are exchanged through these pipes.
During training, the system uses a batch-based approach, where the input data is divided into
smaller batches and each batch is processed by a separate layer in the network. Each layer receives
its input batch from the previous layer through a pipe and then apply the weights and biases to generate
the output. The output is passed to the next layer as input through another pipe.

### Compilation Instructions

To compile the C++ files with the specified executable names, run the following commands in your terminal:

```bash
# Compile each C++ file with specified output names
g++ Ai.cpp -o input       # Compile Ai.cpp to executable 'input'
g++ Ah.cpp -o h           # Compile Ah.cpp to executable 'h'
g++ Ah2.cpp -o h1         # Compile Ah2.cpp to executable 'h1'
# Run the code by writing the command below
./input arg1 arg2
# arg1 specifies if its the forward propogation or backward. For forward arg1 should be '1'.
# arg2 tells the number of hidden layer so it can be any integer values lets say '4'.


