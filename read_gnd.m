function [query_idx, nn_idx, nn_dist] = read_gnd(...
    file_name, topK, type)
% error('check rb is equal to r');

fp = fopen(file_name, 'r');

m = fread(fp, 1, 'int32');
k = fread(fp, 1, 'int32');

if (k > topK)
    k = topK;
end

ele_length = 0;
if (strcmp(type, 'double'))
    ele_length = 8;
elseif (strcmp(type, 'float'))
    ele_length = 4;
else
    error('not supported');
end

query_idx = zeros(m, 1);
nn_idx = zeros(k, m, 'int32');

if (nargout == 3)
    if strcmp(type, 'float')
        nn_dist = zeros(k, m, 'single');
    else
        nn_dist = zeros(k, m, type);
    end
end

for i = 1 : m
%     query_idx(i) = fread(fp, 1, 'int32');
    tmp = fread(fp, 1, 'int32');
 
    
    
    nn_idx(:, i) = fread(fp, k, 'int32');
    if (tmp > k)
        fseek(fp, (tmp - k) * 4, 0);
    end
    
    if (nargout == 3)
        nn_dist(:, i) = fread(fp, k, type);
        if (tmp > k)
            fseek(fp, (tmp - k) * ele_length, 0);
        end
    else
        fseek(fp, tmp * ele_length, 0);
    end
end
fclose(fp);