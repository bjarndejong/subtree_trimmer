# subtree-trimmer

**subtree-trimmer** removes vertices `v` from leaves of `v`-subtrees, whilst maintaining a valid tree decomposition.
It removes `v` from the bag of a leaf of the `v`-subtree if and only if those neighbours of `v` present at the leaf are also present in its parent.
Only the contents of bags are modified — the tree structure remains unchanged.


## Command-Line Usage
```
subtree-trimmer [options] <tree_decomposition.td> <graph.graph>
```
### Arguments

| Argument                  | Description                                               | Required |
|---------------------------|-----------------------------------------------------------|----------|
| `<tree_decomposition.td>`   | Path to the input tree decomposition file (.td). See [`.td` file](#td-file) below. | Yes |
| `<graph.graph>`             | Path to the input graph file (.graph). See [`.graph` file](#graph-file) below.                   |   Yes     |

### Options

| Option                    | Description                                               | Default   |
|---------------------------|-----------------------------------------------------------|----------|
| `--print_td=<true\|false>`   | If `true`, prints the entire `.td` file; if `false`, prints only the solution (`s`) line.                      | `false`     |


### Output

The program writes the trimmed tree decomposition to standard output:
- By default, only the solution (`s`) line is written, in the format:  
    `s td <number_of_bags> <max_bag_size> <number_of_vertices>`
- If `--print_td=true` is set, the full tree decomposition is written in `.td` format.

**Note**:
- Input files are never modified.
- Comments from the original file are not included in the output.


### Example usage

```
subtree-trimmer tdfile.td graphfile.graph
subtree-trimmer graphfile.graph --print_td=true tdfile.td
subtree-trimmer graphfile.graph --print_td=true tdfile.td > tdfile_trimmed.td
```

## File Formats

### `.graph` file
An undirected vertex weighted graph following the METIS format 
- The first line contains three integers: `<number_of_vertices> <number_of_edges> 10`, where   
    - the constant `10` indicates use of vertex weights but not edge weights.
- Vertices are numbered consecutively from `1` to `<number_of_vertices>`
- Each of the following `<number_of_vertices>` lines describes a vertex `v` by `<weight> <neighbour_1> <neighbour_2>` ... :  
    - Here, `<weight>` is the integer weight of vertex `v`, followed by a space-separated list of its neighbours in ascending order.

### `.td` file
The tree decomposition must follow the PACE 2017 format with additional ordering and sorting requirements as specified below:
- **Line types**:
    - `c` — *comment lines*, e.g., `c <text>`
    - `s` — *solution line*, first non-comment line, format:  
    `s td <number_of_bags> <max_bag_size> <number_of_vertices>`
    - `b` — *bag lines*, format:  
    `b <bag_ID> <bag_contents>`
    - *Tree edge lines*, format:  
    `<bag_ID_1> <bag_ID_2>`
- **General constraints**:
    - Bag IDs are continuous integers from `1` to `<number_of_bags>`.
    - The solution line (`s`) must be the first non-comment line; bag lines (`b`) must appear after the solution line and before the tree edge lines.
- **Additional constraints**:
    - Comment lines (`c`) appear only at the beginning of the file.
    - Bag contents within each bag line must be sorted in ascending order.

