function save_gnd(idx, dist, type, file_name)
fp = fopen(file_name, 'w');

[k1 num1] = size(idx);
[k2 num2] = size(dist);

if (k1 ~= k2 || num1 ~= num2)
    error('sfdjsd;');
end

fwrite(fp, num1, 'int32');
fwrite(fp, k1, 'int32');
for i = 1 : num1
	fwrite(fp, k1, 'int32');
    fwrite(fp, idx(:, i), 'int32');
    fwrite(fp, dist(:, i), type);
end

fclose(fp);