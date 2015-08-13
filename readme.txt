
This provides an implementation of the group k-means (gk-means).

gk-means is an algorithm to do clustering or binary code encoding. 
The codes provides routines of how to train the model from the 
training data, how to encode the database points with the model, 
how to do retrieval task, and how to evaluate the retrieval results. 

This code is not quite cleaned, but it should be easy to use and 
should work. 
For any question, please contact me: wjf2006@mail.ustc.edu.cn or
jianfengwang@outlook.com

----------------------------------------------------------------------
Linux: 
1. add the following to ~/.bashrc
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/home/user/code/gkmeans/c_code/bin
if the downloaded folder is /home/user/code/gkmeans

2. Start Matlab and run main_entry.m.

Windows: The main issue is to compile the c++ codes under c_codes/. 
In each sub folder, the project file is provided, but is not well tested. 
A little more effort is required to compile the c++ codes under windows. 
Since no additional library is required, it should not be difficult to 
do it. 

-----------------------------------------------------------------------
Issues:
Q1: libutility.so can not be found 
A1: The file is located in c_code/bin. Thus please add the following in
~/.bashrc


-----------------------------------------------------------------------
Note:
In the demo codes, the data are randomly generated, so if you would like 
to use it in your specific dastaset, please modify the data reading routine, 
including:
1. Line 26-28, how to read the training data to train the model
2. Line 106-107, how to read the test data or the query data
3. Line 87-88, how to read the database points. 

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
}

