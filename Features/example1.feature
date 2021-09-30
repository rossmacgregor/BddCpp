
Feature: Addition

  # Inputs first number and second number are in one line
  Scenario: Sum of two numbers
  
    Given first number is 10.0 and second number is 20
    When user executes SUM
    Then the sum is 30.0
