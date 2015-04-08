
This provides an implementation of the group k-means (gk-means).

gk-means is an algorithm to do clustering or binary code encoding. The codes provides
routines of how to train the model from the training data, how to encode the database
points with the model, how to do retrieval task, and how to evaluate the retrieval 
results. 

For any question, please feel free to contact me: wjf2006@mail.ustc.edu.cn
----------------------------------------------------------------------
Linux: Directly start Matlab and run main_entry.m. 

Windows: The main issue is to compile the c++ codes under c_codes/. In each sub folder, 
    the project file is provided, but is not well tested. A little more effort is required
    to compile the c++ codes under windows. Since no additional library is required, 
    it should not be difficult to do it. 

-----------------------------------------------------------------------
Note:
In the demo codes, the data are randomly generated, so if you would like to use it in 
your specific dastaset, please modify the data reading routine, including:
1. Line 23-25, how to read the training data to train the model
2. Line 103-104, how to read the test data or the query data
3. Line 84-85, how to read the database points. 

------------------------------------------------------------------------
Please also consider to cite our paper if it helps:

\article{WangYYKLW15,
    author    = {Jianfeng Wang and
                 Shuicheng Yan and
                 Yi Yang and
                 Mohan S. Kankanhalli and
                 Shipeng Li and
                 Jingdong Wang},
    title     = {Group {\textdollar}K{\textdollar}-Means},
    journal   = {CoRR},
    volume    = {abs/1501.00825},
    year      = {2015},
    url       = {http://arxiv.org/abs/1501.00825},
    timestamp = {Mon, 02 Feb 2015 14:12:25 +0100},
    biburl    = {http://dblp.uni-trier.de/rec/bib/journals/corr/WangYYKLW15},
    bibsource = {dblp computer science bibliography, http://dblp.org}
}
