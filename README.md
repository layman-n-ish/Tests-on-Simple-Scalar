# Tests-on-Simple-Scalar

In the words of the creators, *"The SimpleScalar tool set is a system software infrastructure used to build modeling applications for program performance analysis, detailed microarchitectural modeling, and hardware-software co-verification"*. An architectural simulator reproduces the behaviour of a computing device. With SimpleScalar, one can simulate programs on various configurations of modern processors, even Out-of-Order (OoO) issue processors, that support non-blocking caches, speculative execution, and state-of-the-art branch prediction. Visit [SimpleScalar](http://www.simplescalar.com/)'s 'Downloads' section to download the tool or the 'Documentation' section to learn about the internals and execution steps.

## Setup

Download the contents of this repository in the same directory as the SimpleScalar toolset source files (obtained from the 'Downloads' section), maintaining the hierarchy. Follow the instructions in the `README` provided by SimpleScalar to build and install. In this exercise, I'll built the toolset on Linux with PISA (Portable ISA) target. To replicate the same, run the below commands in the directory where SimpleScalar is downloaded:

``` 
make config-pisa
make
```

To verfify the build, run any of the various simulator models provided by SimpleScalar (sim-fast, sim-safe, sim-outorder, etc. - more information about each of these in the official documentation) on any of the tests corresponding to the target built:

```
./sim-outorder tests-pisa/bin.little/test-math
```

This should publish the output of the test and various simulation statistics. 

To change the configuration of the processor (cache latencies, cache configuration, number of functional units, branch predictor, etc.), run the below command and supply the parameters to be modified appropiately through command line options or by providing a config file.

```
./sim-outorder -h
```

To be able to execute our own binaries on the simulators, one has to employ a cross-compiler to generate executables for the PISA/Alpha targets. Please read the official documentation on how to setup `gcc-2.6.3` along with the cross-compiler and follow [this](https://github.com/sdenel/How-to-install-SimpleScalar-on-Ubuntu) tutorial by *@sdenel* to solve numerous conflicts during the build process. Once the cross-compiler is setup successfully, one can cross-compile to executables on appropiate targets (PISA/Alpha). Example:

```
bin/sslittle-na-sstrix-gcc -g -O -o mytests/bpred_corr_branch mytests/bpred_corr_branch.c
```

## How to run tests

Run the `run_analysis.sh` bash script to automate the task of running my designed tests on the `sim-outorder` simulator with PISA target.

```
Usage: ./run_analysis.sh [ANALYSIS_TYPE] [TEST_FILE_PATH] [DEBUG]
```
- `ANALYSIS_TYPE` can be 'b' for running branch predictors' test or 'c' for executing analysis on cache-aware algorithms.
- `TEST_FILE_PATH` takes in the path to the executable to run the `sim-outorder` simulator on.
- `DEBUG` only when '1' prints useful stats and messages about the running tests.

**Example**:
```
./run_analysis.sh b mytests/bpred_corr_branch 0

Executing analysis on branch predictors

         Running sim-outorder simulator with 2lev-GAg branch predictor...
         Running sim-outorder simulator with 2lev-gshare branch predictor...
         Running sim-outorder simulator with 2lev-PAg branch predictor...
         Running sim-outorder simulator with 2lev-PAp-concat branch predictor...
         Running sim-outorder simulator with 2lev-PAp-xor branch predictor...
         Running sim-outorder simulator with bimod branch predictor...
         Running sim-outorder simulator with comb branch predictor...
         Running sim-outorder simulator with nottaken branch predictor...
         Running sim-outorder simulator with taken branch predictor...

Extracting vital branch prediction results from the simulator runs...

        Saving bpred:2lev-GAg's results...
        Saving bpred:2lev-gshare's results...
        Saving bpred:2lev-PAg's results...
        Saving bpred:2lev-PAp-concat's results...
        Saving bpred:2lev-PAp-xor's results...
        Saving bpred:bimod's results...
        Saving bpred:comb's results...
        Saving bpred:nottaken's results...
        Saving bpred:taken's results...

```

## Experiments

#### Branch Predictors 

A hypothesis I wanted to prove was how dynamic global predictors should outperform dynamic local predictors on [correlated branch conditionals with a *random* pattern](https://github.com/layman-n-ish/Tests-on-Simple-Scalar/blob/master/mytests/bpred_corr_branch.c); randomness was embedded because only then one can point out the exploitation, by the global predictor, of the auxiliary information about other branch conditionals that global branch history possesses, instead of simply "learning" the pattern itself. This global view lacks in the local predictors ergo their failure in comparison with global predictors. 

For this experiment, I configured various branch predictors ([`myconfig/bpred_*`](https://github.com/layman-n-ish/Tests-on-Simple-Scalar/tree/master/myconfig)) viz. two-level adaptive (and its numerous flavours: GAg, PAg, PAp, gshare), bimodal, hybrid between two-level adaptive and bimodal (named 'comb'), taken, not-taken, etc. Each of these branch predictors were employed sequentially while executing `mytests/bpred_corr_branch.c` which contains the source code for 'executing correlated branch conditionals with random pattern'. Further experiments include modifying parameters of the branch predictor such as history length, table size, etc.  

#### Caches

< About Cache-oblivious algorithms >

## Results and observations

#### Branch Predictors 

Surprisingly, the results don't come out to be consistent with the hypotesis, which can be seen below. 

```
$ ./run_analysis.sh b mytests/bpred_corr_branch.o 1
> [...]

Extracting vital branch prediction results from the simulator runs...

        Saving bpred:2lev-GAg's results...
        Printing bpred:2lev-GAg's metrics:
                bpred_2lev.bpred_addr_rate    0.8181 # branch address-prediction rate (i.e., addr-hits/updates)
                bpred_2lev.bpred_dir_rate    0.8227 # branch direction-prediction rate (i.e., all-hits/updates)
                bpred_2lev.bpred_jr_rate    0.9534 # JR address-prediction rate (i.e., JR addr-hits/JRs seen)

        Saving bpred:2lev-gshare's results...
        Printing bpred:2lev-gshare's metrics:
                bpred_2lev.bpred_addr_rate    0.9055 # branch address-prediction rate (i.e., addr-hits/updates)
                bpred_2lev.bpred_dir_rate    0.9096 # branch direction-prediction rate (i.e., all-hits/updates)
                bpred_2lev.bpred_jr_rate    0.9598 # JR address-prediction rate (i.e., JR addr-hits/JRs seen)

        Saving bpred:2lev-PAg's results...
        Printing bpred:2lev-PAg's metrics:
                bpred_2lev.bpred_addr_rate    0.9471 # branch address-prediction rate (i.e., addr-hits/updates)
                bpred_2lev.bpred_dir_rate    0.9506 # branch direction-prediction rate (i.e., all-hits/updates)
                bpred_2lev.bpred_jr_rate    0.9652 # JR address-prediction rate (i.e., JR addr-hits/JRs seen)

        Saving bpred:2lev-PAp-concat's results...
        Printing bpred:2lev-PAp-concat's metrics:
                bpred_2lev.bpred_addr_rate    0.9371 # branch address-prediction rate (i.e., addr-hits/updates)
                bpred_2lev.bpred_dir_rate    0.9388 # branch direction-prediction rate (i.e., all-hits/updates)
                bpred_2lev.bpred_jr_rate    0.9850 # JR address-prediction rate (i.e., JR addr-hits/JRs seen)

        Saving bpred:2lev-PAp-xor's results...
        Printing bpred:2lev-PAp-xor's metrics:
                bpred_2lev.bpred_addr_rate    0.9610 # branch address-prediction rate (i.e., addr-hits/updates)
                bpred_2lev.bpred_dir_rate    0.9630 # branch direction-prediction rate (i.e., all-hits/updates)
                bpred_2lev.bpred_jr_rate    0.9818 # JR address-prediction rate (i.e., JR addr-hits/JRs seen)

        Saving bpred:bimod's results...
        Printing bpred:bimod's metrics:
                bpred_bimod.bpred_addr_rate    0.9313 # branch address-prediction rate (i.e., addr-hits/updates)
                bpred_bimod.bpred_dir_rate    0.9357 # branch direction-prediction rate (i.e., all-hits/updates)
                bpred_bimod.bpred_jr_rate    0.9558 # JR address-prediction rate (i.e., JR addr-hits/JRs seen)
        Saving bpred:comb's results...
        Printing bpred:comb's metrics:
                bpred_comb.bpred_addr_rate    0.9621 # branch address-prediction rate (i.e., addr-hits/updates)
                bpred_comb.bpred_dir_rate    0.9651 # branch direction-prediction rate (i.e., all-hits/updates)
                bpred_comb.bpred_jr_rate    0.9705 # JR address-prediction rate (i.e., JR addr-hits/JRs seen)

        Saving bpred:nottaken's results...
        Printing bpred:nottaken's metrics:
                bpred_nottaken.bpred_addr_rate    0.3425 # branch address-prediction rate (i.e., addr-hits/updates)
                bpred_nottaken.bpred_dir_rate    0.3425 # branch direction-prediction rate (i.e., all-hits/updates)
                bpred_nottaken.bpred_jr_rate    0.0000 # JR address-prediction rate (i.e., JR addr-hits/JRs seen)

        Saving bpred:taken's results...
        Printing bpred:taken's metrics:
                bpred_taken.bpred_addr_rate    0.3425 # branch address-prediction rate (i.e., addr-hits/updates)
                bpred_taken.bpred_dir_rate    0.3425 # branch direction-prediction rate (i.e., all-hits/updates)
                bpred_taken.bpred_jr_rate    0.0000 # JR address-prediction rate (i.e., JR addr-hits/JRs seen)             
```            

One would expect the global predictors such as gshare & GAg to produce better direction-prediction rate than the local predictors (PAp, PAg, etc.). I've been debugging the obscruity behind the results and shall update here soon.

Complete simulation results can be found under [`results/result_bpred_*`](https://github.com/layman-n-ish/Tests-on-Simple-Scalar/tree/master/results). 

#### Caches



## Future Work
