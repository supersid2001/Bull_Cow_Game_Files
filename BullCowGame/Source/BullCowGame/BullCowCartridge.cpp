// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWorldList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    ValidWords = GetValidWords(Words);
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString &Input) // When the player hits enter
{
    ClearScreen();
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
        return;
    }
    /* If game is over, clear screen and setup game again
    Else check player guess */
    ProcessGuess(Input);
    FBullCowCount Count = GetBullCows(Input);
    PrintLine(TEXT("You have %i bulls and %i cows"), Count.Bulls, Count.Cows);
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = ValidWords[FMath::RandRange(0, ValidWords.Num() - 1)];
    Lives = HiddenWord.Len();
    PrintLine(TEXT("Hello there!"));
    PrintLine(TEXT("Type in your guess. You have %i lives"), Lives);
    PrintLine(TEXT("Press enter to continue."));
    PrintLine(FString::Printf(TEXT("The hidden word is %s\nit is %i characters long"), *HiddenWord, HiddenWord.Len())); //Debug Line
    //Prompt for guess
    PrintLine(FString::Printf(TEXT("Guess the %i letter word"), HiddenWord.Len()));
    bGameOver = false;
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Game Over. Press enter to play again..."));
}

void UBullCowCartridge::ProcessGuess(const FString &Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You win!!"));
        EndGame();
        return;
    }

    if (HiddenWord.Len() != Guess.Len())
    {
        PrintLine(FString::Printf(TEXT("Please enter a word of length %i"), HiddenWord.Len()));
        PrintLine(TEXT("You have %i lives"), Lives);
        return;
    }

    if (!IsIsoGram(Guess))
    {
        PrintLine(TEXT("No repeating letters in your word!"));
        PrintLine(TEXT("You have %i lives"), Lives);
        return;
    }

    if (Lives > 1)
    {
        Lives--;
        PrintLine(TEXT("Incorrect! You have %i lives"), Lives);
        return;
    }

    PrintLine(TEXT("You lose!"));
    PrintLine(TEXT("The hidden word was %s"), *HiddenWord);
    EndGame();
}

bool UBullCowCartridge::IsIsoGram(const FString &Guess) const
{
    for (int32 Index = 0; Index < Guess.Len(); Index++)
    {
        for (int32 JIndex = Index + 1; JIndex < Guess.Len(); JIndex++)
        {
            if (Guess[Index] == Guess[JIndex])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString> &Words) const
{
    TArray<FString> ValidWords;
    for (FString word : Words)
    {
        if (word.Len() >= 4 && word.Len() <= 10 && IsIsoGram(word))
        {
            ValidWords.Emplace(word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString &Guess) const
{
    FBullCowCount Count;
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 HiddenWordIndex = 0; HiddenWordIndex < HiddenWord.Len(); HiddenWordIndex++)
        {
            if (HiddenWord[HiddenWordIndex] == Guess[GuessIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}