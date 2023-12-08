#include <iostream>
#include <utility>
#include "RulesReader.h"
#include "State.h"
#include "helper.h"
#include "NFA.h"
#include "NFA_Generator.h"
#include "InputReader.h"
#include "SetStates.h"
#include "DFA.h"

#ifndef DFA_DFAMINIMIZED_H
#define DFA_DFAMINIMIZED_H

// Map to keep track of the group to 
// which each state belongs during the minimization process.
map<string, int> stateToGroupMap;

/**
 * Loop through the groups and update the stateToGroupMap.
 */
void updateStateToGroupMap(vector<vector<SetStates*>> groups) {
    stateToGroupMap.clear();
    for (int groupNumber = 0; groupNumber < groups.size(); ++groupNumber) {
        for (const auto &state : groups[groupNumber]) {
            stateToGroupMap[state->getStatesIds()] = groupNumber;
        }
    }
}

/**
 * Minimize the DFA represented by the given remaining and final states.
 * @param remStates: Remaining states
 * @param finalStates: Final states
 * @return: Vector of vectors representing the minimized groups of states.
 */
vector<vector<SetStates*>> minimizeDFA(vector<SetStates*> remStates, vector<SetStates*> finalStates) {
    const string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&',.:;<=>?@~(){}[]*/+-|";
    int previousGroupCount = 0;
    vector<vector<SetStates*>> groups {std::move(remStates), std::move(finalStates)};
    updateStateToGroupMap(groups);

    while (previousGroupCount != groups.size()) {
        // Get the number of partitions in this iteration.
        previousGroupCount = static_cast<int>(groups.size());
        vector<vector<SetStates*>> newGroups;

        // Iterate through each group.
        for (const auto& group : groups) {
            // Base case: if this group has only one element, add it to the new groups.
            if (group.size() == 1) {
                newGroups.push_back(group);
                continue;
            }

            vector<SetStates*> identicalStates, remainingStates;
            auto firstState = group[0];
            identicalStates.push_back(firstState);

            // Loop to fill identicalStates and remainingStates vectors.
            for (int i = 1; i < group.size(); i++) {
                auto currentState = group[i];
                bool isDifferent = false;

                // Compare transitions for each input symbol in the alphabet.
                for (char input : alphabet) {
                    bool hasTransition1 = (firstState->nextStates.find(input) != firstState->nextStates.end());
                    bool hasTransition2 = (currentState->nextStates.find(input) != currentState->nextStates.end());

                    // If the two states are different or have transitions to different groups, add to remainingStates.
                    if (hasTransition1 != hasTransition2 ||
                        (hasTransition1 &&
                         (stateToGroupMap[firstState->nextStates[input]->getStatesIds()] !=
                          stateToGroupMap[currentState->nextStates[input]->getStatesIds()]))) {
                        remainingStates.push_back(currentState);
                        isDifferent = true;
                        break;
                    }
                }

                // If the two states are identical, add to identicalStates.
                if (!isDifferent)
                    identicalStates.push_back(currentState);
            }

            newGroups.push_back(identicalStates);
            if (!remainingStates.empty())
                newGroups.push_back(remainingStates);
        }

        groups = newGroups;
        updateStateToGroupMap(groups);
    }

    return groups;
}

/**
 * Generate the minimized DFA based on the final groups of states.
 * @param finalGroups: Vector of vectors representing the minimized groups of states.
 * @return: Pointer to the minimized DFA.
 */
DFA* generateMinDFA(vector<vector<SetStates*>> finalGroups) {
    const string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$%&',.:;<=>?@~(){}[]*/+-|";
    vector<SetStates*> minimizedStates;

    // Concatenate the groups together.
    for (const auto& group : finalGroups) {
        auto* setStates = new SetStates();

        // Loop through the states in the group and insert them into the set.
        for (SetStates* setState : group) {
            setStates->insertSet(setState->states);
        }

        minimizedStates.push_back(setStates);
    }

    // Assign next states for each state in the minimized DFA.
    for (int i = 0; i < finalGroups.size(); i++) {
        SetStates* representativeState = finalGroups[i][0];

        for (char input : alphabet) {
            // Check if the representativeState has a transition with this input.
            if (representativeState->nextStates.find(input) != representativeState->nextStates.end()) {
                string outputStateId = representativeState->nextStates[input]->getStatesIds();
                minimizedStates[i]->nextStates.insert({input, minimizedStates[stateToGroupMap[outputStateId]]});
            }
        }
    }

    // Create the minimized DFA.
    DFA* minimizedDFA = new DFA();
    minimizedDFA->startingState = minimizedStates[0];

    // Add states to the minimized DFA.
    for (SetStates* states : minimizedStates) {
        minimizedDFA->stateIdToSetStatesMap.insert({states->getStatesIds(), states});
    }

    return minimizedDFA;
}

#endif //DFA_DFAMINIMIZED_H
