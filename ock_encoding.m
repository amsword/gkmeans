function mat_compact_B = ock_encoding(ock_model, Xbase)

num_point = size(Xbase, 2);

if num_point <= 5 * 10^6;
    mat_compact_B = UpdateRepresentationCompact(...
        Xbase, ock_model);
else
    batch_size = 10^6;
    batch_num = ceil(num_point / batch_size);
    
    for batch_idx = 1 : batch_num
        idx_start = (batch_idx - 1) * batch_size + 1;
        if idx_start > num_point
            continue;
        end
        
        idx_end = idx_start + batch_size - 1;
        if idx_end > num_point
            idx_end = num_point;
        end
        fprintf('%f\t%f\n', idx_start, idx_end);
        
        subXbase = Xbase(:, idx_start : idx_end);
        sub_mat_compact_B = UpdateRepresentationCompact(...
            subXbase, ock_model);
        if batch_idx == 1
            mat_compact_B = zeros(size(sub_mat_compact_B, 1), ...
                num_point, 'uint8');
        end
        mat_compact_B(:, idx_start : idx_end) = sub_mat_compact_B;
    end
end
