- [Install tools](#org18ce076)
  - [Install build tools](#org76e6817)
  - [Install conda](#org1cf9e81)
- [gatk-power](#org3a914a0)
  - [Clone gatk-power](#org42b8c84)
  - [Build gatk-power](#orge123802)
- [GATK](#orge774062)
  - [Clone GATK](#org7ac1350)
  - [Build GATK](#orgbc91fc9)
- [conda package](#orgdc12361)
  - [Clone conda recipe repository](#org6e77e80)
  - [Build conda package](#org7e21a23)
- [Install conda package](#org145cd09)

This document shows how to build GATK with Power VSX instructions enabled for the Pair HMM algorithm.


<a id="org18ce076"></a>

# Install tools


<a id="org76e6817"></a>

## Install build tools

On a Power LPAR running RHEL, install these build tools:

-   cmake
-   gcc
-   gcc-c++
-   git
-   git-lfs
-   Java 17
-   maven
-   wget

Run

```sh
sudo dnf install cmake gcc gcc-c++ git git-lfs java-17-openjdk \
     java-17-openjdk-devel maven wget
```

You may have multiple versions of Java installed, so you'll want to make java and javac default to using Java 17:

```sh
sudo alternatives --config java
sudo alternatives --config javac
```

and select the lines for Java 17.


<a id="org1cf9e81"></a>

## Install conda

If you don't have conda installed on your build system, you'll need to download and install it.<sup><a id="fnr.1" class="footref" href="#fn.1" role="doc-backlink">1</a></sup>

```sh
mkdir -p ~/miniconda3
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-ppc64le.sh \
     -O ~/miniconda3/miniconda.sh
bash ~/miniconda3/miniconda.sh -b -u -p ~/miniconda3
rm ~/miniconda3/miniconda.sh
~/miniconda3/bin/conda init bash
~/miniconda3/bin/conda init zsh
```

You'll want to exit restart your shell to pick up shell settings for conda.


<a id="org3a914a0"></a>

# gatk-power


<a id="org42b8c84"></a>

## Clone gatk-power

Run

```sh
git clone https://github.com/davemq/gatk-power.git
```


<a id="orge123802"></a>

## Build gatk-power

```sh
cd gatk-power
./gradlew publishToMavenLocal
sudo mkdir -p /usr/java/packages/lib
sudo cp ./build/native/libvsx_pairhmm.so /usr/java/packages/lib
cd ..
```


<a id="orge774062"></a>

# GATK


<a id="org7ac1350"></a>

## Clone GATK

Run

```sh
git clone --branch nonative https://github.com/davemq/gatk.git
```


<a id="orgbc91fc9"></a>

## Build GATK

```sh
cd gatk
./gradlew bundle
cp ./build/gatk-4.5.0.0-26-gb0ac604-SNAPSHOT.zip /tmp
cd ..
```


<a id="orgdc12361"></a>

# conda package


<a id="org6e77e80"></a>

## Clone conda recipe repository

```sh
git clone --branch gatk4.5 https://github.com/davemq/biobuilds.git
```


<a id="org7e21a23"></a>

## Build conda package

```sh
cd biobuilds
conda install conda-build
conda config --append channels conda-forge
conda build gatk4/4.5.0.0
cd ..
```

The conda package for GATK 4.5.0.0 should now be in ~/miniconda3/conda-bld/linux-ppc64le as gatk-4.5.0.0-0.tar.bz2.


<a id="org145cd09"></a>

# Install conda package

To use the new conda package, you can either copy it to a common conda repository you already use, or add "local" to your channel list, or specify `-c local` when installing in a conda environment.

```sh
conda config --prepend channels local
conda install gatk=4.5.0.0
```

or

```sh
conda install -c local gatk=4.5.0.0
```

## Footnotes

<sup><a id="fn.1" class="footnum" href="#fnr.1">1</a></sup> From <https://docs.anaconda.com/miniconda/>, modified for ppc64le.
