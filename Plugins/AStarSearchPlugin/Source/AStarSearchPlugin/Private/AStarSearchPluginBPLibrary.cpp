// Copyright Epic Games, Inc. All Rights Reserved.

#include "AStarSearchPluginBPLibrary.h"

#include <queue>
#include <tuple>
#include <array>
#include <stack>

#include "SWarningOrErrorBox.h"
#include "Containers/BasicArray.h"

// Creating a shortcut for tuple<int, int, int> type
typedef std::tuple<double, int, int> Tuple;

UAStarSearchPluginBPLibrary::UAStarSearchPluginBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

// A Utility Function to check whether given cell (row, col)
// is a valid cell or not.
bool UAStarSearchPluginBPLibrary::IsValid(const size_t & Row, const size_t& Col,
            const FVector2D& Point)
{ // Returns true if row number and column number is in
    // range
    if (Row > 0 && Col > 0)
        return (Point.X >= 0) && (Point.X < Row)
            && (Point.Y >= 0)
            && (Point.Y < Col);

    return false;
}

// A Utility Function to check whether the given cell is
// blocked or not
bool UAStarSearchPluginBPLibrary::IsBlocked(const TArray<TArray<AGridCell*>>& Grid,
                const FVector2D& Point)
{
    // Returns true if the cell is blocked else false
    return Grid[Point.X][Point.Y]->GetIsWall();
}

// A Utility Function to check whether destination cell has
// been reached or not
bool UAStarSearchPluginBPLibrary::IsDestination(const FVector2D& Src, const FVector2D& Dest)
{
    return Src == Dest;
}

// A Utility Function to calculate the 'h' heuristics.
double UAStarSearchPluginBPLibrary::CalculateHValue(const FVector2D& Src, const FVector2D& Dest)
{
    // h is estimated with the two points distance formula
    return sqrt(pow((Src.X - Dest.X), 2.0)
                + pow((Src.Y - Dest.Y), 2.0));
}

// A Utility Function to trace the path from the source
// to destination
void UAStarSearchPluginBPLibrary::TracePath(const TArray<TArray<AGridCell*>>& Grid, const FVector2D& Dest)
{
	UE_LOG(LogActor, Warning, TEXT("\nThe Path is "));
    int row = Dest.X;
    int col = Dest.Y;
    std::stack<FVector2D> Path;
 
	auto next_node = Dest;
	do {
		Path.push(next_node);
		next_node = Grid[row][col]->parent;
		row = next_node.X;
		col = next_node.Y;
	} while (Grid[row][col]->parent != next_node);

	Path.emplace(row, col);
	while (!Path.empty()) {
		auto p = Path.top();
		Path.pop();
		UE_LOG(LogActor, Warning, TEXT("-> (%f,%f) "), p.X, p.Y);
	}
}

TArray<FVector2D> UAStarSearchPluginBPLibrary::AStarSearch(const FVector2D& Start, const FVector2D& End, AGridSpawner* GridSpawner)
{
	TArray<FVector2D> Positions;
	auto Grid = GridSpawner->GetGrid();
	int32 Row = Grid.Num();
	int32 Col = Grid[0].Num();
	
    // If the source is out of range
	if (!IsValid(Row, Col, Start)) {
		UE_LOG(LogActor, Warning, TEXT("Source is invalid\n"));
		return Positions;
	}

	// If the destination is out of range
	if (!IsValid(Row, Col, End)) {
		UE_LOG(LogActor, Warning, TEXT("Destination is invalid\n"));
		return Positions;
	}

	// Either the source or the destination is blocked
	if (IsBlocked(Grid, Start) || IsBlocked(Grid, End)) {
		UE_LOG(LogActor, Warning, TEXT("Source or the destination is blocked\n"));
		return Positions;
	}

	// If the destination cell is the same as source cell
	if (IsDestination(Start, End)) {
		UE_LOG(LogActor, Warning, TEXT("We are already at the destination\n"));
		return Positions;
	}

	// Create a closed list and initialise it to false which
	// means that no cell has been included yet This closed
	// list is implemented as a boolean 2D array
	
	TArray<TArray<bool>> closedList;
	closedList.SetNum(Row);
	
	for(auto& R : closedList)
	{
		R.Init(false, Col);
	}
	
	double i, j;
	// Initialising the parameters of the starting node
	i = Start.X, j = Start.Y;
	Grid[i][j]->f = 0.0;
	Grid[i][j]->g = 0.0;
	Grid[i][j]->h = 0.0;
	Grid[i][j]->parent = { i, j };

	/*
	Create an open list having information as-
	<f, <i, j>>
	where f = g + h,
	and i, j are the row and column index of that cell
	Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
	This open list is implemented as a set of tuple.*/
	std::priority_queue<Tuple, std::vector<Tuple>, std::greater<Tuple>> openList;

	// Put the starting cell on the open list and set its
	// 'f' as 0
	openList.emplace(0.0, i, j);

	// We set this boolean value as false as initially
	// the destination is not reached.
	while (!openList.empty()) {
		const Tuple& p = openList.top();
		// Add this vertex to the closed list
		i = std::get<1>(p); // second element of tuple
		j = std::get<2>(p); // third element of tuple

		// Remove this vertex from the open list
		openList.pop();
		closedList[i][j] = true;
		/*
				Generating all the 8 successor of this cell
					N.W   N   N.E
					   \  |  /
						\ | /
					W----Cell----E
						/ | \
					   /  |  \
					S.W   S   S.E

				Cell-->Popped Cell (i, j)
				N --> North	 (i-1, j)
				S --> South	 (i+1, j)
				E --> East	 (i, j+1)
				W --> West		 (i, j-1)
				N.E--> North-East (i-1, j+1)
				N.W--> North-West (i-1, j-1)
				S.E--> South-East (i+1, j+1)
				S.W--> South-West (i+1, j-1)
		*/
		for (int add_x = -1; add_x <= 1; add_x++) {
			for (int add_y = -1; add_y <= 1; add_y++) {
				FVector2D neighbour(i + add_x, j + add_y);
				// Only process this cell if this is a valid one
				if (IsValid(Row, Col, neighbour)) {
					// If the destination cell is the same as the current successor
					if (IsDestination(neighbour,End)) {
						// Set the Parent of the destination cell
						Grid[neighbour.X][neighbour.Y]->parent = { i, j };
						UE_LOG(LogActor, Warning, TEXT("The destination cell is found\n"));
						TracePath(Grid, End);
						return Positions;
					}
					// If the successor is already on the
					// closed list or if it is blocked, then
					// ignore it. Else do the following
					else if (!closedList[neighbour.X][neighbour.Y] && !IsBlocked(Grid, neighbour)) {
						double gNew, hNew, fNew;
						gNew = Grid[i][j]->g + 1.0;
						hNew = CalculateHValue(neighbour, End);
						fNew = gNew + hNew;

						// If it isn’t on the open list, add
						// it to the open list. Make the
						// current square the parent of this
						// square. Record the f, g, and h
						// costs of the square cell
						//			 OR
						// If it is on the open list
						// already, check to see if this
						// path to that square is better,
						// using 'f' cost as the measure.
						if (Grid[neighbour.X][neighbour.Y]->f == -1
							|| Grid[neighbour.X][neighbour.Y]->f > fNew) {
							openList.emplace(fNew, neighbour.X, neighbour.Y);

							// Update the details of this cell
							Grid[neighbour.X][neighbour.Y]->g = gNew;
							Grid[neighbour.X][neighbour.Y]->h = hNew;
							Grid[neighbour.X][neighbour.Y]->f = fNew;
							Grid[neighbour.X][neighbour.Y]->parent = { i, j };
						}
					}
				}
			}
		}
	}

	// When the destination cell is not found and the open
	// list is empty, then we conclude that we failed to
	// reach the destination cell. This may happen when the
	// there is no way to destination cell (due to
	// blockages)
	
	UE_LOG(LogActor, Warning, TEXT("Failed to find the Destination Cell\n"));
	return Positions;
}