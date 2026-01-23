					insurance label classification – explanation
The goal of this project is to take a list of 10k companies and automatically tag them with the right insurance labels based on their descriptions and metadata. since there isn t a predefined "correct" answer key, i focused on making a program that is fast, explainable, and actually makes sense when you look at the results. I tried to focus more on solving the actual problem than on using complex algorithms just for the sake of it.

At first i thought about reading the companies one by one and scanning every label against them, but that would be way too slow for a big dataset (simplified time complexity : O(N × L × WC) WC = Word count). instead i decided to build an inverted index. I index all words first and map them to the company id and the field where they were found. this uses more memory but it makes lookups happen almost instantly (time complexity : O(N × WC)).

I convert all text to lowercase and strip out punctuation so the matching stays consistent. i also filtered out common noise words like service, company, and business (still feel like this is a bit hardcoded). without this the signal becomes very noisy because those words are in almost every description.

For the labeling i used a weighted scoring system. not all matches are equal, so if a keyword from a label shows up in the niche or category, it gets more points than if it is just in the description. this makes the results much more accurate because the niche usually defines the business better. after summing the points, if a score passes a threshold, the label is assigned. I chose this because it is very transparent you can always explain exactly why a label was given to a company, unlike a black box model.

I considered using semantic bridges like saying that "farm" implies "food" but i decided not to do it. to me that feels like the program already "knows" the domain, which is a form of hardcoding. I preferred to keep the logic generic so it works on any taxonomy or any list of companies without needing me to feed it extra hints.

The programworks best for concrete activities like manufacturing or farming. it performs worse for abstract labels like compliance or consulting where the keywords are more vague. since there is no formal accuracy score, i validated the performance by manually checking a random sample of companies to see if the labels made sense. it is not a perfect classifier, but it is scalable, easy to explain, and works as a solid baseline that can be improved over time.

The program is designed not to touch your original data. it does not update the old companies file; instead, it generates a brand new CSV file with the classification results. i did this to prevent any data corruption in case the program crashes or has a runtime failure. the output file will be named with a classified_ prefix.


							HOW TO RUN IT !
		-----IMPORTANT:	The new File will be created in the insurances folder--------
	to make things easy, i created a Makefile that handles the compilation and execution.
 1. using the makefile
	-if you just want to run everything with the default files, use: make run
		this will compile the code, run the classification using ml_insurance_challenge.csv and insurance_taxonomy.csv from the insurances folder, and then clean up the executable.
		-also if you want to test with different files and run with makefile modify the COMPANIES and LABELS variables from the MAkefile


	---------- IF YOU DON T HAVE MAKEFILE INSTALLED----------
2. running manually 
	if you want to run it manually or use different filenames, follow these steps:
	compile the program by typing in the terminal:
	-> g++ -Wall -Wextra -Werror main.cpp helper_functions.cpp Company.cpp -o challenge
	run the executable with the two file arguments:
	./challenge "your_companies_file.csv" "your_labels_file.csv"
	
	---------- IF YOU DON T HAVE MAKEFILE , C++ AND/OR A COMPILER INSTALLED ----------
3. Run with Docker 
	docker-compose up --build
	important note: the input files should be located in the insurances folder for the program to find them correctly. this is how the argument parsing is set up in the code.

	---------IF YOU DON T HAVE MAKEFILE , C++ ,A COMPILER  OR DOCKER INSTALLED ----------
		IDK - just watch this : https://www.youtube.com/watch?v=SNKjJvAXwJo

5. CLEAN UP
	-just type make fclean


