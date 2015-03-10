1 Introduction
There are two versions of the LINE package, the one is for Windows and the other is for LINUX.
There are five files for each version:
(1) line.cpp: the codes of the LINE model.
(2) reconstruct.cpp: the codes for reconstructing sparse networks into dense ones as described in section 4.3 of the paper.
(3) normalize.cpp: the codes for normalizing the original vertex embeddings, which will set the L2 norm of the embeddings as 1.
(4) concatenate.cpp: the codes to concatenate the 1st-order embeddings and the 2nd-order embeddings.
(5) train_youtube.bat / train_youtube.sh: the demo to train vertex embeddings on the reconstructed Youtube network. The dataset can be downloaded at http://socialnetworks.mpi-sws.mpg.de/data/youtube-links.txt.gz

2 Environment
The codes of LINE rely on some external packages to generate random numbers for the edge-sampling algorithm.
The Windows version of LINE rely on the BOOST package which can be downloaded at http://www.boost.org/
The Linux version of LINE rely on GSL package which can be downloaded at http://www.gnu.org/software/gsl/

3 Format of the network data
The network data contains serveral lines, each line represents a DIRECTED edge in the network.
More specifically, each line has the following format "<u> <v> <w>", meaning an edge from <u> to <v> with weight as <w>.
<u> <v> and <w> are seperated by ' ' or '\t' (blank or tab)
For UNDIRECTED edge, the user should use two DIRECTED edges to represent it.
Here is an example:
-------------------
good the 3
the good 3
good bad 1
bad good 1
bad of 4
of bad 4
-------------------
