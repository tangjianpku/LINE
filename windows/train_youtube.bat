@echo off

reconstruct.exe -train net_youtube.txt -output net_youtube_dense.txt -depth 2 -k-max 1000
line.exe -train net_youtube_dense.txt -output vec_1st_wo_norm.txt -binary 1 -size 128 -order 1 -negative 5 -samples 10000 -threads 40
line.exe -train net_youtube_dense.txt -output vec_2nd_wo_norm.txt -binary 1 -size 128 -order 2 -negative 5 -samples 10000 -threads 40
normalize.exe -input vec_1st_wo_norm.txt -output vec_1st.txt -binary 1
normalize.exe -input vec_2nd_wo_norm.txt -output vec_2nd.txt -binary 1
concatenate.exe -input1 vec_1st.txt -input2 vec_2nd.txt -output vec_all.txt -binary 1