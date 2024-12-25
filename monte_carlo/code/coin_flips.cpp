def getCoinProb(headsProb, numFlips):
    prob = 0.0
    for flip in range(numFlips):
        currentFlip = random.random()
        if currentFlip <= headsProb: #Probability of Heads
            prob += 1
    return prob / numFlips
