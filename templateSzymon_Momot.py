#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on a sunny day


Student Name: Szymon Momot

Student ID: R00204278

Cohort: CS2

"""
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('Movies-1.csv') # Read the CSV file into a DataFrame called df

def task1():

    # Find the number of main-genres and Find the most popular and the least popular genres
    specific_genres = df['main_Genre'].nunique()
    most_popular_genre = df['main_Genre'].mode().values[0]
    least_popular_genre = df['main_Genre'].value_counts().idxmin()

    # show the top 8 popular genres using a bar plot
    top_genres = df['main_Genre'].value_counts().nlargest(8)
    plt.figure(figsize=(12, 6))
    plt.barh(top_genres.index, top_genres.values)
    plt.title('Top 8 Popular Genres')
    plt.xlabel('Number of Movies')
    plt.ylabel('Main Genre')
    plt.show()

    # Print the results
    print(f'The Total Number of Unique Main-Genres is: {specific_genres}')
    print(f'The Most Popular Main-Genre is: {most_popular_genre}')
    print(f'The Least popular main-genre is: {least_popular_genre}')
 #call function
task1()

def task2():
    # Split the genres in the "genre" column, expand them into separate columns,than stack and than reset the index
        genres = df["Genre"].str.split(",", expand=True).stack().reset_index(level=1, drop=True)                                                                    # stack result DataFrame to have a single column, and reset the index.
        most_common_genre = genres.value_counts().idxmax()
        least_common_genre = genres.value_counts().idxmin()
    # Print the results
        print(f"Most common genres: {most_common_genre}")# most common
        print(f"Least common genre: {least_common_genre}")# least common

 #call function
task2()

def task3():

    # take numeric values from "Runtime" to convert to numeric
    df["Runtime"] = pd.to_numeric(df["Runtime"].str.extract("(\d+)", expand=False), errors="coerce")

    # Identify the outliers
    q1 = df["Runtime"].quantile(0.25)
    q3 = df["Runtime"].quantile(0.75)
    IQR = q3 - q1 #interquartile range

    outliers = df[(df["Runtime"] < (q1 - 1.5 * IQR)) | (df["Runtime"] > (q3 + 1.5 * IQR))]
    outliers_titles = outliers["Title"].tolist()

    #boxplot
    plt.figure(figsize=(12, 8))
    plt.boxplot(df["Runtime"].dropna(), vert=True)
    plt.title("Boxplot of Movie Durations")
    plt.xlabel("Duration (minutes)")
    plt.show()

    # prints the outliers
    print("Outliers in Movie Duration:")
    for title in outliers_titles:
        print(title)


# call function
task3()



def task4():
    # Check for null values in 'Number of Votes' and 'Rating'
    null_values = df[['Number of Votes', 'Rating']].isnull().sum()
    # Fill null values with the mean of existing values for each attribute
    df['Number of Votes'].fillna(df['Number of Votes'].mean(), inplace=True)
    df['Rating'].fillna(df['Rating'].mean(), inplace=True)

    # Visualization of the relationship between 'Number of Votes' and 'Rating' as a scatter plot
    plt.figure(figsize=(12, 8))
    plt.scatter(x=df['Number of Votes'], y=df['Rating'], alpha=0.5)
    plt.title('Scatter Plot: Number of Votes vs Rating')
    plt.xlabel('Number of Votes')
    plt.ylabel('Rating')
    plt.show()
    print("Null Values:\n", null_values)

    # The null values in 'number of votes', 'rating' may exist due to various reasons some movies might not have gotten any votes or ratings
    # another problem could be data collection error or any information that could go missing
    # to work with these null values i have changed them with the mean with existing values for each attribute

 # call function
task4()



def task5():
    # Load main_genre.csv
    main_genre_df = pd.read_csv("main_genre.csv", encoding='ISO-8859-1')

    # Clean and lower-case the terms in main_genre.csv
    main_genre_df = main_genre_df.apply(lambda x: x.str.lower().str.replace(r'[’“”.-]', ''))

    # Clean and lower-case the synopsis in Movies.csv
    df['Synopsis'] = df['Synopsis'].str.lower().str.replace(r'[’“”.-]', '')

    # Iterate over main genres
    for main_genre in main_genre_df.columns:
        # Select movies with synopsis associated with the main genre
        selected_movies = df[df['Synopsis'].str.contains('|'.join(main_genre_df[main_genre]), na=False)]

        if not selected_movies.empty:
            # Determine the main genre with the highest frequency
            most_frequent_main_genre = selected_movies['main_Genre'].value_counts().idxmax()

            # Print
            print(f"{main_genre}: {most_frequent_main_genre}")

 # call function
task5()



def task6():

    # Extract four-digit years from the 'Release Year' column
    df['Release_Year'] = df['Release Year'].str.extract(r'(\d{4})')

    # Convert 'Release Year' to datetime
    df['Release_Year'] = pd.to_datetime(df['Release_Year'], format='%Y', errors='coerce')

    # Calculate the average rating for each year
    average_rating_by_year = df.groupby(df['Release_Year'].dt.year)['Rating'].mean()

    # Plotting Average Rating Over Time as a bar chart with rotated x-axis labels
    plt.figure(figsize=(35, 6))
    average_rating_by_year.plot(kind='bar', color='skyblue', rot=45)  # Rotate x-axis labels by 45 degrees
    plt.title('Average Movie Rating Over Time (Bar Chart)')
    plt.xlabel('Release Year')
    plt.ylabel('Average Rating')
    plt.tight_layout()  # Adjust layout to prevent clipping of labels
    plt.show()

# The output will be a bar chart titled 'Average Movie Rating Over Time' with the x-axis representing the release years and the y-axis representing the average rating.
# Each bar in the chart corresponds to a specific year, and the height of the bar indicates the average rating for movies released in that year.
# The x-axis labels will be rotated by 45 degrees for better visibility for the user.
# The plot will provide a visual representation of how the average movie rating has changed over the years.


 # call function
task6()