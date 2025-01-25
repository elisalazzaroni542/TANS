# Essential Instructions

To compile the program from scratch and test the simulation, navigate to the project directory and enter the command  `make all`:

```bash
$ cd project
$ make all
```
This will compile the classes and execute the macros. For subsequent runs, you can use the command `make run`, which will skip recompilation of the classes:

```bash
$ make run
```

# Detailed Instructions

### Overview of Commands

The `make all` and `make run` commands internally invoke other commands defined in the makefile. These commands can be listed using:

```
$ make list
Available commands:
all
analysis
clean
compile
list
reco
run
show
sim
```

### Cleaning the Project

To clean the project of compilation files, dependencies, and ACLiC dictionaries, use the `make clean` command:

```
$ make clean
done.
```

An empty output indicates no compilation files were generated yet.

### Compilation

To manually compile the project, use `make compile`:
```
$ make compile
Compiling simulation files...
```
A full compilation output example is provided at the end of the document.

### Running the Simulation

Run the simulation using `make sim`:
```
$ make sim
Processing sim.C+(123, 1000000, true, true, "custom")...
Done, running simulation...
Total events: 1000000
Total events with hits: 998755
Done.
Real time 0:00:35, CP time 35.360
Simulation done.
```
### Customizing Simulation Parameters

The `make sim` command accepts the following arguments:

*SEED*: Specifies the seed for random processes (default: 123).

*EVENTS*: Specifies the number of events to simulate (default: 1M).

*MS*: Enables/disables multiple scattering corrections (true by default).

*NOISE*: Enables/disables noise simulation (true by default).

*DISTR*: Specifies the multiplicity distribution (custom by default). Options: gaus, uni, custom.

Example with custom arguments:

```
$ make sim MS=false NOISE=false DISTR=gaus
Processing sim.C+(123, 1000000, false, false, "gaus")...
Done, running simulation...
Total events: 1000000
Total events with hits: 999979
Done.
Real time 0:00:27, CP time 27.680
```
### Reconstruction

Run the reconstruction step with `make reco`:
```
$ make reco
Processing reco.C+(1000000, 0.01, 0.5)...
Done, running reconstruction...
Max reco vertices: 79
Done.
Real time 0:00:13, CP time 13.250
Reconstruction done.
```
### Reconstruction Parameters

The `make reco` command accepts the following arguments:

*EVENTS*: Specifies which file to reconstruct based on the number of simulated events (default: 1M).

*PHICUT*: Sets the maximum φ distance for track association (default: 0.01 rad).

*WSIZE*: Specifies the reconstruction window size (default: 0.5 cm).

*NOISE*: Enables/disables reconstruction with noise (default: true).

### Analysis

Run the analysis step with `make analysis`:
```
$ make analysis
Processing analysis.C+(1000000)...
Info in <TCanvas::Print>: png file ../plots/ZDiff_vs_ZGen.png has been created
Info in <TCanvas::Print>: png file ../plots/MultiplicityDistributions.png has been created
...
Real time 0:00:02, CP time 2.180
```

### Display Results

To display the generated results, use `make show`:
```
$ make show
```



## make compile Example Output
```
$ make compile
Compiling simulation files ...
Processing / home / dcristof / Desktop / TANS / project / src / Event . cxx +...
Info in < TUnixSystem :: ACLiC >: creating shared library / home / dcristof /
Desktop / TANS / project / src / Event_cxx . so
( Event ) Name : Random3 Title : Random number generator : Mersenne Twister
Processing / home / dcristof / Desktop / TANS / project / src / Trajectory . cxx +...
Info in < TUnixSystem :: ACLiC >: creating shared library / home / dcristof /
Desktop / TANS / project / src / Trajectory_cxx . so
( Trajectory ) Name : Random3 Title : Random number generator : Mersenne
Twister
Processing / home / dcristof / Desktop / TANS / project / src / HitPoint . cxx +...
Info in < TUnixSystem :: ACLiC >: creating shared library / home / dcristof /
Desktop / TANS / project / src / HitPoint_cxx . so
( HitPoint ) Name : Random3 Title : Random number generator : Mersenne
Twister
Processing / home / dcristof / Desktop / TANS / project / src / Point . cxx +...
Info in < TUnixSystem :: ACLiC >: creating shared library / home / dcristof /
Desktop / TANS / project / src / Point_cxx . so
( Point ) Name : Point Title :
Done .
```

