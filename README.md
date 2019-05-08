# SPN and Image Completion

*Sum-product network* (**SPN**) implementation and its application on image completion.

## Content

[About This Repo](#about-this-repo)

- [Motivation](#motivation)
- [Target](#target)
- [About SPN](#about-spn)
- [About Dataset](#about-dataset)
- [About Results](#about-results)

[Documents of Code](#documents-of-code)

- [Code](#code)
- [Callgraph of Program](#callgraph-of-program)

[How to Run](#how-to-run)

- [Set Up Environment](#set-up-environment)
- [File Path](#file-path)
- [Compilation](#compilation)

[Results](#results)

- [Evaluation](#evaluation)
- [Image Comparison](#images-comparison)

[Timeline](#timeline)

[Author](#author)

[Acknowledgement](#acknowledgement)

[License](#license)

---

### About this repo

#### Motivation

The complexity of the partition function is a key limiting factor in graphical model inference and learning. Sum-product network is a new kind of architecture which is tractable, compared to Bayesian network and Markov random fields. Recently, SPN have appealed to more and more researchers. This repo contains all files of my undergraduate thesis, a project to implement SPN and reproduce the results of image completion.

#### Target

The target of this project is to reproduce the results of the application to image completion proposed in the original paper[1].

#### About SPN

SPNs are directed acyclic graphs with variables as leaves, sums, and products as the internal node, and probability as weighted edges. This architecture is tractable and aims to resolve the limiting factor, the complexity of the graphical model. Any tractable graphical model can be cast into SPNs. However, SPNs are more general.

#### About Dataset

Two dataset are used in this program, following the application of orginal paper[1]:

- [Caltech-101](http://www.vision.caltech.edu/Image_Datasets/Caltech101/): The dataset contains 101 categories of pictures of objects, with about $40$ to $800$ images per category.[2]

- [Olivetti](https://www.cl.cam.ac.uk/research/dtg/attarchive/facedatabase.html): The dataset contains a set of face images taken between April 1992 and April 1994 at AT&T Laboratories Cambridge with each image in size $64 \times 64$[3].

#### About Results

The models learned from the dataset will be stored in `results/<DOMAIN>/models`, which DOMAIN will either be `caltech` or `olivetti`. And the images for left and bottom completion will be output to `results/<DOMAIN>/completion`. The original and completed images will be aligned by separation with $10$px gaps but no zero intensity.

### Documents of Code

#### Code

1. **common**
   1. `my_mpi`: Use **OpenMPI** to support the messaging in a parallel program. It means that this program will use parallel architecture to accelerate computing.
   2. `parameter`: Control parameters for EM algorithm, SPN, and evaluation.
   3. `timer`: Manage the time to help calculate the time spent on computation.
   4. `utils`: Some helper functions to access time, print log, and do some numeric process.
2. **evaluation**
   1. `dataset`: Read and process data from the dataset. Input are images and will be divided into training and test sets.
   2. `eval`: Conduct evaluation over the dataset.
   3. `image_completion`: Conduct image completion, which is the application.
   4. `run`: Control the program, which is the main function.
3. **spn**
   1. `decomposition`: Decompose the regions.
   2. `generative_learning`: Conduct the learning process to generative the model.
   3. `instance`: Record the mean and variance of an instance, which is calculated from the dataset.
   4. `node`: Define the node, to provide the base class of the sum node and the product node.
   5. `prod_node`: Define the product node, derived from `node`.
   6. `sum_node`: Define the sum node, derived from `node`.
   7. `region`: Compute the mean and variance of regions in the picture(for this image completion application), as well as the MAP.
   8. `SPN`: Define the Sum-product network, including a root, functions of learning and applications. These functions are implemented via calling other modules.

#### Callgraph of Program

This picture will show the call graph between modules of this program.

![Call graph of SPN](./figures/spn_callgraph.png)

### How to Run

Under folder `Implementation`, run the following commands to compile the executable program:

#### Set Up Environment

**openMPI** is needed to provide the parallel computing environmet. Please visit [open-mpi official website](https://www.open-mpi.org) for details if not available.

#### File Path

To make the program read files correctly, please modify the file path in `dataset.cpp`, `eval.cpp`, and `run.cpp` to correct path before compiling.

#### Compilation

Please use **MPI** wrapped **g++** compilers, either **openmpi-wrapped gcc** or **Intel compilers** with **OpenMPI** library.

Compile `run` first and run the binary for results.

Compile `eval` second and run the bianry to evaluate the MSE.

Run the following commands to get the executable program:

```sh
make run # to run the learning process
make eval # to do evaluation
```

Run the following commands to reproduce the experiment:

```sh
mpic++ -n 102 run -d C  #for caltech, 2 groups with 50 slaves and 1 master per group
# or
mpic++ -n 51 run -d O  # for olivetti, 1 group with 50 slaves and 1 master
```

### Results

Results will be stored temporally under `Implementation/results`.

#### Evaluation

Here are MSE of some categories:

|  Category | MSE in Left Half | MSE in Bottom Half |
|:---------:|:----------------:|:------------------:|
| accordion |       7629       |        8291        |
|   brain   |       2799       |        3057        |
| butterfly |       4621       |        4390        |
|    cup    |       5434       |        5278        |
|  Olivetti |       1084       |        1156        |

#### Image Comparison




For models, please see **mdl** files in `Implementation/results/<DOMAIN>/models`

For completion data, please see **dat** files in `Implementation/results/<DOMAIN>/completions`

For more evaluation results, please see `evaluation_mse.txt` in `Implementation/results`

### Reference

[1] Poon, Hoifung and Domingos, Pedro. Sum-Product Networks: A new deep architecture. In Proceedings of the Conference on Uncertainty in Artificial Intelligence (UAI), 2011. [Code](http://spn.cs.washington.edu/spn/).

[2] [Caltech 101 Website](http://www.vision.caltech.edu/Image_Datasets/Caltech101/)

[3] [Olivetti Faces Website](https://www.cl.cam.ac.uk/research/dtg/attarchive/facedatabase.html)

### Timeline

- [x] Implementation of SPN (*Dec. 28, 2018*)

- [x] Documents of Code (*Jan. 31, 2019*)

- [x] Implementation of rest part(*Mar. 4, 2019*)

- [x] Reproduce of the Original Application (*Apr. 24, 2019*)

- ~~[ ] Further Exploration and Optimization (*Apr. 27, 2019*)~~

- [x] Interim Report (*Apr. 4, 2019*)

- [x] Thesis Draft(*May 6, 2019*)

- [ ] Final Defence (*May 20, 2019*)

### Author

Yilin ZHENG  

E-mail: 11510506@mail.sustech.edu.cn

### Acknowledgement

Supervisors:

- Prof. Shan He(UBir)
- Prof. Ke Tang(SUSTech)

Other:

- PhD. candidate Weifeng Li(UBir)
- PhD. Xiaofen Lu(SUSTech)
- Phd. Guiying Li(SUSTech)

### License

**MIT**