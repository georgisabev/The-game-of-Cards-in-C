#include <stdio.h>
#include <stdlib.h>

struct Cards
{
    char value;
    char color;
};

unsigned int get_value(char value_of_card)
{
    if ('0' <= value_of_card && value_of_card <= '9')
        return value_of_card - '0';
    else if (value_of_card == 'J')
        return 10;
    else if (value_of_card == 'Q')
        return 12;
    else if (value_of_card == 'K')
        return 15;
    else if (value_of_card == 'A')
        return 20;
}

void bubble_sort(struct Cards* player, int size) {

    for (int step = 0; step < size - 1; ++step)
    {
        for (int i = 0; i < size - step - 1; ++i)
        {
            if (get_value(player[i].value) <= get_value(player[i + 1].value))
            {
                char temp_value = player[i].value;
                char temp_color = player[i].color;
                player[i].value = player[i + 1].value;
                player[i].color = player[i + 1].color;
                player[i + 1].value = temp_value;
                player[i + 1].color = temp_color;
            }
            if (get_value(player[i].value) == get_value(player[i + 1].value) && player[i].color < player[i + 1].color)
            {
                char temp_color_for_inside_same_cards = player[i].color;
                player[i].color = player[i + 1].color;
                player[i + 1].color = temp_color_for_inside_same_cards;
            }
        }
    }

}

void print_winner(struct Cards *cards, unsigned int number_of_cards, int number_of_winner)
{
    printf("\033[0;33m");
    printf("Player %d wins :‑) \n", number_of_winner);
    printf("\033[0m");

    bubble_sort(cards, number_of_cards);

    for (int i = 0; i < number_of_cards; i++)
    {
        if(cards[i].color == 'D' || cards[i].color == 'H')
            printf("\033[0;31m");

        if (i == number_of_cards - 1)
            printf("(%c, %c)", cards[i].value, cards[i].color);
        else
            printf("(%c, %c), ", cards[i].value, cards[i].color);
        printf("\033[0m");
    }
}



unsigned int max_of_four(unsigned int player1, unsigned int player2, unsigned int player3, unsigned int player4)
{
    return player4 < (player3 < (player1 < player2 ? player2 : player1) ? (player1 < player2 ? player2 : player1) : player3) ? (player3 < (player1 < player2 ? player2 : player1) ? (player1 < player2 ? player2 : player1) : player3) : player4;
}

void check_for_winner_after_dealing(struct Cards *player1_cards, struct Cards *player2_cards, struct Cards *player3_cards, struct Cards *player4_cards)
{
    unsigned int player1_sum_of_cards = 0, player2_sum_of_cards = 0, player3_sum_of_cards = 0, player4_sum_of_cards = 0;

    for (int i = 0; i < 13; i++)
    {
        player1_sum_of_cards += get_value(player1_cards[i].value);
        player2_sum_of_cards += get_value(player2_cards[i].value);
        player3_sum_of_cards += get_value(player3_cards[i].value);
        player4_sum_of_cards += get_value(player4_cards[i].value);
    }

    unsigned int max = max_of_four(player1_sum_of_cards, player2_sum_of_cards, player3_sum_of_cards, player4_sum_of_cards);

    if (max == player1_sum_of_cards)
        print_winner(player1_cards, 13, 1);
    else if (max == player2_sum_of_cards)
        print_winner(player2_cards, 13, 2);
    else if (max == player3_sum_of_cards)
        print_winner(player3_cards, 13, 3);
    else if (max == player4_sum_of_cards)
        print_winner(player4_cards, 13, 4);
}

short int check_for_winer(struct Cards *player1_cards, struct Cards* player2_cards, struct Cards* player3_cards, struct Cards* player4_cards, unsigned int turn)
{
    int player1_number_of_repetition = 0, player2_number_of_repetition = 0, player3_number_of_repetition = 0, player4_number_of_repetition = 0;
    unsigned int current_max = 0, current_best_player = 0;

    for (unsigned int i = 0; i < turn - 1 && player1_number_of_repetition < 3 && player2_number_of_repetition < 3 && player3_number_of_repetition < 3 && player4_number_of_repetition < 3; i++)
    {
        if (player1_cards[i].value == player1_cards[turn - 1].value)
            player1_number_of_repetition++;
        if (player2_cards[i].value == player2_cards[turn - 1].value)
            player2_number_of_repetition++;
        if (player3_cards[i].value == player3_cards[turn - 1].value)
            player3_number_of_repetition++;
        if (player4_cards[i].value == player4_cards[turn - 1].value)
            player4_number_of_repetition++;
    }

    if (player1_number_of_repetition == 3 && current_max < player1_cards[turn - 1].value)
    {
        current_best_player = 1;
        current_max = player1_cards[turn - 1].value;
    }
    if (player2_number_of_repetition == 3 && current_max < player2_cards[turn - 1].value)
    {
        current_best_player = 2;
        current_max = player2_cards[turn - 1].value;
    }
    if (player3_number_of_repetition == 3 && current_max < player3_cards[turn - 1].value)
    {
        current_best_player = 3;
        current_max = player3_cards[turn - 1].value;
    }
    if (player4_number_of_repetition == 3 && current_max < player4_cards[turn - 1].value)
    {
        current_best_player = 4;
        current_max = player4_cards[turn - 1].value;
    }


    return current_best_player;
}

void dealing_cards(char* cards, struct Cards *player1_cards, struct Cards* player2_cards, struct Cards* player3_cards, struct Cards* player4_cards)
{

    for (int i = 0, turn = 1; turn <= 13; i += 16, turn++)
    {
        player1_cards[turn - 1].value = cards[i], player1_cards[turn - 1].color = cards[i + 2];
        player2_cards[turn - 1].value = cards[i + 4], player2_cards[turn - 1].color = cards[i + 6];
        player3_cards[turn - 1].value = cards[i + 8], player3_cards[turn - 1].color = cards[i + 10];
        player4_cards[turn - 1].value = cards[i + 12], player4_cards[turn - 1].color = cards[i + 14];

        int player_with_hand = check_for_winer(player1_cards, player2_cards, player3_cards, player4_cards, turn);
        if (player_with_hand == 1)
        {
            print_winner(player1_cards, turn, 1);
            return;
        }
        if (player_with_hand == 2) {
            print_winner(player2_cards, turn, 2);
            return;
        }
        if (player_with_hand == 3)
        {
            print_winner(player3_cards, turn, 3);
            return;
        }
        if (player_with_hand == 4)
        {
            print_winner(player4_cards, turn, 4);
            return;
        }
    }

    check_for_winner_after_dealing(player1_cards, player2_cards, player3_cards, player4_cards);
}

int main()
{

    char *file_name = malloc(sizeof(char) * 200);
    scanf("%s", file_name);

    FILE* file_with_cards = fopen(file_name, "rb");

    fseek(file_with_cards, 0, SEEK_END);
    size_t size_of_file = ftell(file_with_cards);
    fseek(file_with_cards, 0, SEEK_SET);

    char* cards = malloc(sizeof(char) * size_of_file);
    fread(cards, sizeof(char), size_of_file, file_with_cards);


    struct Cards *player1_cards = malloc(sizeof(struct Cards) * 13),
            *player2_cards = malloc(sizeof(struct Cards) * 13),
            *player3_cards = malloc(sizeof(struct Cards) * 13),
            *player4_cards = malloc(sizeof(struct Cards) * 13);

    dealing_cards(cards, player1_cards, player2_cards, player3_cards, player4_cards);

    return 0;
}

