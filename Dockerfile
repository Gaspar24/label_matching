# use standard c++ library
FROM gcc:latest

#set s the working directory
WORKDIR /app

# copy from the folder in the container
COPY . .

# RUN mkdir -p insurances
# COPY insurances/* insurances/

# compile using make
RUN make 

# run the program using the paths to the files inside the insurances folder
CMD ["./challenge", "ml_insurance_challenge.csv", "insurance_taxonomy.csv"]