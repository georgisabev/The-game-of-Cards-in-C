# The-game-of-Cards-in-C
A simple game of cards where whoever first gets dealed 4 cards of same value wins.

Играта на карти се изпълнява чрез помощта на следните функции:

------------------------------

1. unsigned int get_value(char value_of_card) -> превръща чаровете от файла в числа. Използва се за пресмятане на стойности и сравнения.

2. void bubble_sort(struct Cards* player, int size) -> подрежда в низходящ ред картите. Не е базовата версия на алгоритъма, защото подрежда и по сила на боята на картите.

3. void print_winner(struct Cards *cards, unsigned int number_of_cards, int number_of_winner) -> Принтира победител в конзолата

4. unsigned int max_of_four(unsigned int player1, unsigned int player2, unsigned int player3, unsigned int player4) -> При край на играта без победител проверя кой то играчите има най-висок сбор на картите.

5. void check_for_winner_after_dealing(struct Cards *player1_cards, struct Cards *player2_cards, struct Cards *player3_cards, struct Cards *player4_cards) -> След край на игграта определя кой е победител чрез max_of_four

6. void dealing_cards(char* cards, struct Cards *player1_cards, struct Cards* player2_cards, struct Cards* player3_cards, struct Cards* player4_cards) -> Разпределя картите от файла в структурите на всеки играч и проверява дали на някой от играчите не се е паднал чифт..

7. int main() -> Чете тестето от файл и започва програмата.


