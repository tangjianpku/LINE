# LINE: Large-scale information network embedding

#Introduction

This is the LINE toolkit developed for embedding very large-scale information networks. It is suitable to a variety of networks including directed, undirected, binary or weighted edges. The LINE model is quite efficient, which is able to embed a network with millions of vertices and billions of edges on a single machine within a few hours.

Contact: Jian Tang, Microsoft Research Asia, jiatang@microsoft.com, tangjianpku@gmail.com



#Usage

We provide both the Windows and LINUX versions. The package uses some external packages to generate random numbers for the edge-sampling algorithm in the LINE model. For Windows version, we use the BOOST package, which can be downloaded at http://www.boost.org/; for LINUX, we use the GSL package, which can be downloaded at http://www.gnu.org/software/gsl/

Network Input:



#Citation

@inproceedings{tang2015line,
  title={LINE: Large-scale Information Network Embedding.},
  author={Tang, Jian and Qu, Meng and Wang, Mingzhe and Zhang, Ming and Mei, Qiaozhu},
  booktitle={WWW},
  year={2015},
  organization={ACM}
}
