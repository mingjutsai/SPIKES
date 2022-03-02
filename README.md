# SPIKES

**SPIKES**: Knowledge of the host-specific properties of the spike protein is of crucial importance to understand the adaptability of severe acute respiratory syndrome coronavirus-2 (SARS-CoV-2) to infect multiple species and alter transmissibility, particularly in humans. Here, we propose a spike protein predictor SPIKES incorporating with an inheritable bi-objective combinatorial genetic algorithm to identify the biochemical properties of spike proteins and determine their specificity to human hosts. We suggest that alterations of these amino acids between human and animal coronaviruses may provide insights into the development and transmission of SARS-CoV-2 in human and other species and support the discovery of targeted antiviral therapies.

## Input Data

FASTA format (e.g., [example.fa](example.fa))

## Getting start

```shell
git clone https://github.com/mingjutsai/SPIKES
cd SPIKES
```

build AAIndex

```shell
cd src/aaindex
make
```

build LIBSVM

```shell
cd src/libsvm_320
make
```

## Example of running SPIKES

create a new folder for the new analysis

```shell
mkdir example_output
```

```shell
perl SPIKES_main.pl example.fa example_output
```

## Result of ESA-UbiSite

results of example.fa -> example_output/ESAUbiSite_prediction.html
[ESAUbiSite_prediction.html](https://nctuiclab.github.io/ESA-UbiSite/example_output/SPIKES_prediction_results.csv)

## Dataset

### The format of datasets

- 1st: accession number of proteins
- 2nd: protein sequence
- 3rd: prediction score (normalized probabilities less than 0.5 are predicted to be human's Spike protein, those greater than 0.5 are predicted to be non-human's Spike protein.)

### Precalculating Dataset

- [All Human Spike protein sequences in FASTA format](dataset/Human.fasta)
- [Prediction results of All Human Spike protein sequences](dataset/Human_SPIKES_prediction_results.csv)
- [All non-Human Spike protein sequences in FASTA format](dataset/nHuman.fasta)
- [Prediction results of All non-Human Spike protein sequences](dataset/nHuman_SPIKES_prediction_results.csv)

## Citing ESA-UbiSite

Tracking the amino acid changes of spike proteins across diverse host species of severe acute respiratory syndrome coronavirus 2. _iScience_, 2022 [PMID:[34877480](https://www.ncbi.nlm.nih.gov/pubmed/34877480)]

## Contact

- Srinivasulu Yerukala Sathipati: sathipathi.srinivasulu@marshfieldclinic.org
- Ming-Ju Tsai: mingjutsai@hsl.harvard.edu
