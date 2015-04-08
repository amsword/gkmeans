function [sub_dim_start_idx sub_dim_lengths] = dim_split(dim, num_partitions)

sub_dim_lengths = zeros(num_partitions, 1);
sub_dim_length_lower = floor(dim / num_partitions);
sub_dim_lengths(:) = sub_dim_length_lower;
num_larger = mod(dim, num_partitions);
if num_larger > 0
    sub_dim_lengths(1 : num_larger) = sub_dim_length_lower + 1;
end

sub_dim_start_idx = cumsum([1; sub_dim_lengths]);
