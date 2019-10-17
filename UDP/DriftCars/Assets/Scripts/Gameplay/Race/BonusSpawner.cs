using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BonusSpawner : MonoBehaviour
{
    [SerializeField]
    GameObject bonusPrefab;
    [SerializeField, Range(1.0f, 30.0f)]
    float bonusRespawnDelay = 1.0f;
    [SerializeField]
    float yOffset = 1.0f;

    GameObject bonusInst = null;
    Coroutine respawnCoroutine = null;


    // Use this for initialization
    void Start ()
    {
        InstantiateNewBonus();
	}

    private void InstantiateNewBonus()
    {
        bonusInst = Instantiate(bonusPrefab, transform.position + Vector3.up * yOffset, transform.rotation);
        Bonus bonus = bonusInst.GetComponent<Bonus>();
        bonus.OnPickedUp += Bonus_OnPickedUp;
    }

    private void Bonus_OnPickedUp()
    {
        respawnCoroutine = StartCoroutine(RespawnBonus());
    }

    IEnumerator RespawnBonus()
    {
        yield return new WaitForSeconds(bonusRespawnDelay);
        InstantiateNewBonus();
        respawnCoroutine = null;
    }

    private void OnDestroy()
    {
        if (bonusInst != null)
        {
            Bonus bonus = bonusInst.GetComponent<Bonus>();
            bonus.OnPickedUp -= Bonus_OnPickedUp;
        }
        if (respawnCoroutine != null)
            StopCoroutine(respawnCoroutine);
    }

    private void OnDrawGizmos()
    {
        Gizmos.DrawWireSphere(transform.position, 1f);
    }
}
