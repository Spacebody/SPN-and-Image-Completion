# Undergraduate-thesis

*Sum-product network* (**SPN**) implementation and its application on image completion and gene dataset.

## Content

[About this repo](###about-this-repo)

[Documents of Code](###documents-of-code)

[Callgraph of Program](###callgraph-of-program)

[How to Run](###how-to-run)

[Results](###results)

[Timeline](###timeline)

[Author](###author)

[Acknowledgement](###acknowledgement)

[License](###license)

---

### About this repo

TODO

### Documents of Code

1. **common**
   1. `my_mpi`: Use **OpenMPI** to support the messaging in a parallel program. It means that this program will use parallel architecture to accelerate computing.
   2. `parameters`: Control parameters for EM algorithm, SPN, and evaluation.
   3. `timer`: Manage the time to help calculate the time spent on computation.
   4. `utils`: Some helper functions to access time, print log, and do some numeric process.
2. **evaluation(TODO)**
   1. `dataset`: 
   2. `eval`
   3. `image_completion`
   4. `run`
3. **spn**
   1. `decomposition`
   2. `generative_learning`
   3. `instance`
   4. `node`
   5. `prod_node`
   6. `sum_node`
   7. `region`
   8. `SPN`

### Callgraph of Program

This picture will show the call graph of this program.

TODO

### How to Run

TODO

### Results

TODO

### Timeline

- [x] Implementation of SPN (*Dec. 28, 2018*)

- [ ] Documents of Code (**Jan. 31, 2019**)

- [ ] Implementation of rest part(**Mar. 1, 2019**)

- [ ] Reproduce of the Original Application (*Feb. 14, 2019*)

- [ ] Further Exploration and Optimization (*Mar. 15, 2019*)

- [ ] Thesis (*April 15, 2019*)

- [ ] Presentation and Defense (*TBD*)

### Author

Yilin ZHENG  

E-mail: 11510506@mail.sustc.edu.cn

### Acknowledgement

Prof. He & Prof. Tang

Dr. Lee

### License

**MIT**