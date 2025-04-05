# Deep Learning with Python - Notes

## Mathematical building blocks of a neural network

Any machine learning system is built with the tensor data structure. Tensor is a container for data and has dimensions. Depending on number of dimensions, there could be different types of tensors - scalars, vectors, matrices and rank-3 or higher dimensional tensors.

1. `Scalars`: They are single number and can be of any type `float32`, `float64`, `int32` etc.

```py
import numpy as np
x = np.array(12)
x.ndim

>> 0
```

2. `Vectors`: They are array of numbers. Vector is a 1D tensor.

```py
x = np.array([5, 12, 11, 10, 6])
x.ndim

>> 1
```

The above vector has 5 entries so it has a dimension of 5. Vector dimension is different than tensor dimension. A 5D vector has only 1 axis and 5 elements along one of this axis, whereas 5D tensor has 5 axis and may have any number of dimensions (no of elements) along any of these axis.

_Dimensionality_ can indicate number of elements along one axis or number of axis. To avoid confusion, we call the later as rank instead of dimension, so the name rank-1 tensor.

3. `Matrix`: An array of vectors is matrix or 2D tensor or rank-2 tensor. Usually the axis are called as rows and columns respectively.

```py
x = np.array([[5, 12, 20, 23],
              [4, 7, 10, 11],
              [2, 11, 4, 8]])
x.ndim

>> 2
```

The entries from the first axis is called `rows` and second axis is called `columns`.

The first row in the above example is `[5, 12, 20, 23]` and first column is `[5, 4, 2]`.

4. Rank-3 Tensor and more...

When you pack the matrices along an axix, they become rank-3 tensor. When you pack the rank-3 tensor, then you get rank-4 tensor and so on.

### Key Attributes

Tensor has 3 attributes -

1. Number of axis or rank - For instance 3D tensor has 3 axes, and a matrix has 2 axes.
2. Shape: Shape is a tuple containing the dimension of each axes in the tensor. For, instance the matrix has shape of `(3, 4)`.
3. Data type or `Dtype`: Data type of the tensor means type of any single element in the tensor. It could be `float32`, `float64`, `int32`, `uint8` etc.

Let's download the MNIST dataset before looking at the tensor attributes-

```py
from tensorflow.keras.datasets import mnist
(train_images, train_labels), (test_images, test_labels) = mnist.load_data()
```

```py
train_images.ndim
>> 3

train_images.shape
>> (60000, 28, 28)

train_images.dtype
>> dtype('unint8')
```

### Manipulating numpy tensors

In Numpy, we can access any value with index. To access, the 3rd value of a numpy tensor along the 0-axis in the `train_images` tensor, we have to do the following-

```py
# Access the 3rd value in the train_images
image = train_images[2, :, :]
image.shape

>> (28, 28)
```

As you can see, to access the 3rd value in the tensor you use the index 2 (because it is 0 based). Each value in the `train_images` tensor along 0-axis represents a matrix which has shape `28X28`. They are images of (28, 28) shape and each pixel in the image has value ranging from 0 to 255.

To manipulate tensors in numpy, we can use slicing as well to manipulate the tensors in case we want a range of values from the tensor. In slicing, to access the values from 3rd to 100th images in the `train_images`.

```py
my_slice = train_images[3:101, :, :]
my_slice.shape

>> (98, 28, 28)
```

Slicing has a start index and end index (exclusive). The end index is not included in the result, so to get what we want, we have to specify `3:101`. To get all values from any axis, we can use `:`.

### Batch

The first axis in the tensor or the axis-0 is considered as the sample axis, as the samples or examples are kept in this axis.

We do not process the entire dataset instead we process a batch of dataset everytime.

The first batch will look like -

```py
train_images[0 : 128]
```

Second batch will be -

```py
train_images[128 : 256]
```

The n-th batch will look like this -

```py
train_images[(n-1)*128 : n*128]
```

When considering, the first axis is called the _batch dimension_ or _batch axis_.

### Real world examples of data tensors

The real world data will fall in any of the following category when working with machine learning systems-

1. Vector data: Rank-2 tensor with shape `(samples, features)` where each sample is a vector of numerical attributes which are `features`.
2. Timeseries data: Rank-3 tensor with shape `(samples, timestamps, features)`, where each sample is a sequence (of length `timestamps`) of feature vectors.
3. Images: Rank-4 tensors with shape `(samples, height, width, channels)`, where each sample represents a 2D-grid of pixels and each pixel is a feature vector(with dimension `channels`).
4. Videos: Rank-5 tensors with shape `(samples, frames, height, width, channels)`, where each sample is a sequence (of length `frames`) of images.

### The engine of neural networks - gradient-based optimizations

Each neural network transforms it's input with the following rule-

```py
import numpy as np

def relu(x):
    return np.max(x, 0)

W = np.random.rand(10, 3)
b = np.random.rand(1)

x = np.random.rand(10, 3)
output = relu(np.matmul(W.T, x) + b)
output
```

```
array([2.54284845, 2.96710633, 3.50213165])
```

`W` and `b` are tensors that are attributes of the layer.

`W` is called `kernel` and `b` is known as `bias`.

They are `trainable parameters` or `weights` of the neural network.

At first, they have random values (`random initialization`). Then with training they get more better values which results in a better result.

This gradual adjustment to weights with feedback signal is called `training`.

The following happens with a `training loop`, repeat the steps untill the loss seems sufficiently less -

1. Draw a batch of training data that contains `x`, the input data and `y_true`, the labels of each training sample.
2. Run the model with input `x`, get prediction for the input `y_pred` called `forward propagation`.
3. Calculate the loss for the prediction by comparing it to the ground truth `y_true`, which penalizing the bad predictions.
4. Update the weights according to the loss calculated in the previous step to slightly reduce the loss.

We use `gradient-descent` optimization technique to find the optimal coefficient values of the neural network that reduces the loss value.

The technique is based on the fact that if `z = x + y`, a small change of `y` will result in a small change in `z`. If you know the direction of the change in `x`, you will know the direction of change in `z`.

Mathematically, you say these functions are `differentiable`.

If you combine multiple such functions then you will have big function that is still `differentiable`.

This applies to mapping between model coefficients to model loss value, if you made a small change in the coefficients of the model then the loss value will also have a small predictable change.

You can use a tool called `gradient` to find out the direction of change happened to the loss value. Then you can compute this gradient to decide the direction to which you can change your coefficients to lower the loss value.
