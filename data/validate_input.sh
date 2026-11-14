
# Input validation
for input_file in ./{secret,sample}/*.in
do

    python ../input_format_validators/validate.py < "$input_file"
    exit_code=$?

    if [ "$exit_code" -ne 42 ]
    then

        echo "Invalid formatting for input file $input_file"
        exit 1

    fi

done

#output file creation


#for input_file in *.in
#do

#    output_file="${input_file%.in}.ans"

    # insert correct script here!
#    echo "Processing $input_file -> Generating $output_file"
#    python temp.py < "$input_file" > "$output_file"

#done

