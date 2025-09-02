# subtree-trimmer

**subtree-trimmer** removes vertices `v` from leaves of `v`-subtrees, whilst maintaining a valid tree decomposition.
It removes `v` from the bag of a leaf of the `v`-subtree if and only if those neighbours of `v` present at the leaf are also present in its parent.
Only the contents of bags are modified — the tree structure remains unchanged.


## Command-Line Usage
```
subtree-trimmer [options] <tree_decomposition.td> <graph.gr>
```
### Arguments

| Argument                  | Description                                               | Required |
|---------------------------|-----------------------------------------------------------|----------|
| `<tree_decomposition.td>`   | Path to the input tree decomposition file (.td). See [`.td` file](#td-file) below. | Yes |
| `<graph.gr>`             | Path to the input graph file (.gr). See [`.gr` file](#gr-file) below.                   |   Yes     |

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
subtree-trimmer tdfile.td graphfile.gr
subtree-trimmer graphfile.gr --print_td=true tdfile.td
subtree-trimmer graphfile.gr --print_td=true tdfile.td > tdfile_trimmed.td
```

## File Formats

### `.gr` file
An graph file must follow the PACE 2017 format with additional ordering and sorting requirements as specified below: 
- **Line types**:
    - `c` — *comment lines*, e.g., `c <text>`   
    - `p` — *problem line*, first non-comment line, format:  
    `p tw <number_of_vertices> <number_of_edges>`
    - *Graph edge lines*, format:  
    `<vertex_ID_1> <vertex_ID_2>`
- **General constraints**:
    - Vertex IDs are continuous integers from `1` to `<number_of_vertices>`
    - The problem line (`p`) must be the first non-comment line.
    - Each edge appears exactly once.
- **Additional constraints**:
    - Comment lines (`c`) appear only at the beginning of the file, before the problem line (`p`).
    - Graph edge lines must be sorted

### `.td` file
The tree decomposition file must follow the PACE 2017 format with additional ordering and sorting requirements as specified below:
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

