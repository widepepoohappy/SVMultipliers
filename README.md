
# WALLACE Trial Repository

Production from RTL to MTO

⚠️ **This multiplier only works on unsigned numbers** ⚠️

## How to use the Wallace generator

1. Set the bitwidth of the two operators (A, B) inside `WallaceAlgorithm.cpp`.
2. Set the numbers you want to multiply.
3. Execute `make` in the main folder.
4. Run `./main` to perform the simulation and generate the hardware.

## Output

You can find the generated hardware inside the `./sv` folder:

### 1. Main Unit
- `MUL`: top-level module

### 2. Components
- `ANDoperator`: performs bitwise AND between all bits
- `WALLACEtree`: adds the partial products
- `RCA`: sums the last stage of the Wallace tree  
   *Note: RCA can later be replaced with a PPA for faster operation*


### 3. Synthesis
- `Fusion compiler`: This is a fusion compiler based synthesis
based on 38nm synopsys example tech node. Tech, tlu+ and map files can be found in the ref dir.
FC lib (ndm) is being called `mul_unit.nlib` containing the essential NDMs provided by synopsys example RTL to GDS flow.
A simple rectangle is being used as floorplan, aiming for 0.5 utilization (no strict constrants imposed)
- `Main command`: `compile_fusion` used to synthesize the code

To open Fusion compiler perform `make -f rm_setup/Makefile console` in the trial dir

To load a block perform in the FC shell `open_block -edit mul_unit:Top_final_opto`

---

 **ENJOY!**

