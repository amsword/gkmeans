function rec = mexResultRecall(retrieval_result, ...
        ground_truth_file, ...
        k, ...
        NBase)
    
    rec = ResultPerformance(...
        retrieval_result, ...
        ground_truth_file, ...
        int32(k), ...
        NBase);